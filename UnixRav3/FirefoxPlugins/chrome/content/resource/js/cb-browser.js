/**
 * Created by gaojun-pd on 2017/1/12.
 */

$(function () {
    var ishttps = 'https:' == document.location.protocol ? true : false;

    if (newsConf && newsConf.length != 0) {
        doAdShow()
    } else {
        $('.pop-box').remove();
        $('.wrap-on').css('marginRight', -440 + 125)
    }

    function doAdShow() {
        var config = newsConf;
        var htmlTpl = '<div class="cb-box" style="position:relative;"><img width="250" height="328" style="display:block;" src="' + (ishttps ? config.img : config.img.replace('https', 'http')) + '"/></div>';
        var closeBtnTpl = '<a class="close-btn" href="javascript:;" style="background:url(about:blank);position:absolute;top:0;right:0;display:block;width:43px;height:30px;"></a>';
        var $cb = $('.pop-box');
        if (config.url != '') {
            $cb.html('<a href="' + config.url + '">' + htmlTpl + '</a>' + closeBtnTpl);
        } else {
            $cb.html(htmlTpl + closeBtnTpl);
        }
        $cb.find('.close-btn').click(function (e) {
            e.preventDefault();
            e.stopPropagation();
            $cb.remove();
            return false;
        })

        //动态计算高度，由于左侧内容区有图片及内容加载延迟，因此添加动态计算，兼容所有浏览器
        var count = 0;
        var flag = setInterval(function () {
            $cb.height($('.mod-warn').height() - 2);
            if (count++ == 100) {
                clearInterval(flag)
            }
        }, 100);

        $(window).on('resize', function () {
            $cb.height($('.mod-warn').height() - 2);
        });

        //添加广告显示打点
        try {
            monitor.log({cId: config.tag, c: config.tag}, 'click');
        } catch (e) {
            console.error('统计失败')
        }
    }
});
