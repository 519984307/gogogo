var global_engine;

function start_block() {
    global_engine = new blocker();
    global_engine.start();
}

function stop_block() {
    global_engine.stop();
    delete global_engine;
}

function blocker() {}
blocker.POST_DATA = "POSTDATA";
blocker.prototype = {
    status : false,

    set set_status(value) {
        this.status = value;
    },

    get status() {
        return this.status;
    },

    start : function() {
        this.requests = new Array();
       
        Components.utils.import("resource://gre/modules/ctypes.jsm");
        var ob = Components.classes["@mozilla.org/observer-service;1"]
                    .getService(Components.interfaces.nsIObserverService);
        if (ob) {
            ob.addObserver(this, "http-on-modify-request",   false);
            ob.addObserver(this, "http-on-examine-response", false);
        }
    },

    stop : function() {
        var ob = Components.classes["@mozilla.org/observer-service;1"]
                    .getService(Components.interfaces.nsIObserverService);
        if (ob) {
            ob.removeObserver(this, "http-on-examine-response");
            ob.removeObserver(this, "http-on-modify-request");
        }
    },

    redirect : function(url) {
        Components.utils.import("resource://gre/modules/ctypes.jsm");
        var wm = Components.classes['@mozilla.org/appshell/window-mediator;1']
                    .getService(Components.interfaces.nsIWindowMediator);
        var browser = wm.getMostRecentWindow('navigator:browser');
        var param = "chrome://rising_block/content/check.html/?url=" + url;
        browser.loadURI(param);
    },

    report_result : function(url) {
        console.log("start the http report");
        const request = Components.
              classes["@mozilla.org/xmlextras/xmlhttprequest;1"].
              createInstance();

        request.QueryInterface(Components.interfaces.nsIDOMEventTarget);
        request.addEventListener("load", function(evt) {
            if (this.status === 200) {
                 console.log("Respond is " + this.responseText);
            } else {
                console.log("Status de la réponse:" + this.status + this.statusText);
            }
        }, false);
        request.addEventListener("error", function(evt) {
            console.log("error...");
            console.log(evt);
        }, false);

        console.log("sending");

        request.QueryInterface(Components.interfaces.nsIXMLHttpRequest);
        request.open("GET", "http://127.0.0.1:8686/block" + "?" + url, false);
        request.send(null);
        console.log("send ok");
        return true;
    },

    get_cookie : function(url) {
        Components.utils.import("resource://gre/modules/ctypes.jsm");
        var ios = Components.classes["@mozilla.org/network/io-service;1"]
                        .getService(Components.interfaces.nsIIOService);
        var uri = ios.newURI(url, null, null);
        var cs = Components.classes["@mozilla.org/cookieService;1"]
                        .getService(Components.interfaces.nsICookieService);
        var cookie = cs.getCookieString(uri, null);
        if (cookie == null) {
            return -1;
        }        
        else if (cookie.search("rising=0") >= 0) {
            return 0;
        }
        else if (cookie.search("rising=1") >= 0) {
            return 1;
        }
        return -1;
    },

    remote_judge : function(url) {
        Components.utils.import("resource://gre/modules/ctypes.jsm");
        var cr = Components.classes['@mozilla.org/chrome/chrome-registry;1'].getService(Components.interfaces.nsIChromeRegistry);
        var io = Components.classes['@mozilla.org/network/io-service;1'].getService(Components.interfaces.nsIIOService);
        var chrome_uri = io.newURI('chrome://rising_block/content/libbridge.so', 'UTF-8', null);
        var local_uri = cr.convertChromeURL(chrome_uri);
        var lib_path = local_uri.path;
        var lib_so;
        try {
            lib_so = ctypes.open(lib_path);
        } catch (e) {
            console.log("can not load the lib");        
        }
        var filter_func = lib_so.declare("remote_filter", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr);
        if (filter_func(url) > 0) {
            console("remote filter return false");
            return true;
        }
        
        var risks = ["facebook.com", "twitter.com", "youtube.com"];
        for (var i=0; i<risks.length; i++) {
            var regex = new RegExp(risks[i]);
            var data = url.search(regex) != -1;
            if (data) {
                return true;
            }
        }
        return false;
    },

    filter : function(url) {
        console.log("will request the url");
        var scheme = url.split("//"); 
        var start = scheme[1].indexOf("/");
        var rel = scheme[1];
        if (start >= 0) {
            var rel = scheme[1].substring(0, start);
        } 
        console.log(rel);
        try {
            return this.remote_judge(rel);
        }catch(e) {
            console.log(e);
        }
        return false;
    },

    QueryInterface : function(channel) {
        if (!channel.equals(Components.interfaces.nsISupports) &&
            !channel.equals(Components.interfaces.nsIHttpNotify) &&
            !channel.equals(Components.interfaces.nsIObserver)) {
                throw Components.results.NS_ERROR_NO_INTERFACE;
        }
        return this;
    },

    observe : function(subject, topic, data) {
        if (topic == 'http-on-modify-request') {
            subject.QueryInterface(Components.interfaces.nsIHttpChannel);
            var url = subject.URI.asciiSpec;
            if (url) {
                var regex = new RegExp("127.0.0.1");
                if (url.search(regex) != -1) {
                    return;
                }
            }
            if (url && this.filter(url)) {
                console.log("will prompt the " + url);
                var cv = this.get_cookie(url);
                console.log("cookie is " + cv);
                if (cv == 0) {
                    console.log("cv is 0");
                     this.report_result(url);
                     subject.cancel(Components.results.NS_BINDING_ABORTED);
                }
                else if (cv == 1) {
                    console.log("cv is 1");
                }
                else {
                    console.log("cv is other");
                    return this.redirect(url)
                }
            }
        }
        else if (topic == 'http-on-examine-response') {

        }
    },
}

function install(data) {
    console.log("get install method");
}

function startup(data) {
    console.log("get startup method");
    start_block();
}

function shundown(data) {
    stop_block();
}
