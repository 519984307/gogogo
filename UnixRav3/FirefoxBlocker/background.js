// get one time
var denysource = "http://127.0.0.1:8686";
function GetText(url, cb){
  //创建xhr对象  
  var xhr;
  if(window.XMLHttpRequest){
      xhr = new XMLHttpRequest();
  }
  //发送请求
  xhr.open('get',browser.extension.getURL(url),true);
  try{
    xhr.send();
  }catch(e){
    console.log(e);
  }
  //同步接受响应
  xhr.onreadystatechange = function(){
	if(xhr.readyState == 4 && xhr.status == 200){
	 cb(xhr.responseText);
     }	
  }
  
}

var filterData = {url:[]};
GetText(denysource + '/deny.json', function(dat){
	filterData = JSON.parse(dat);
	console.log(filterData);
});




browser.webRequest.onBeforeRequest.addListener(
  (details)=>{
    var o = details.url.indexOf(denysource + '/deny.json');
    if(o == 0){
      return {cancel:false};
    }
    // console.log(filterData);
    
    // console.log(filterData);
    for(var i in filterData.urls){
      var x = details.url.indexOf(filterData.urls[i]);

      if( x >= 0){
        var j = browser.extension.getURL('deny.html?' + details.url);
        GetText(denysource + '/block?' + details.url, true);
        return {redirectUrl:j};
      }
    }
    
   return {cancel:false};
  },
  {urls: ["<all_urls>"]},
  ["blocking"]
);
