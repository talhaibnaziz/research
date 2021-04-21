var pbgrdad = pbgrdad || (
 function(){
    
    var zone = [];
    var show = function(domId,iframe,config){
        jQuery("#"+domId).append(iframe);
        if(config.fixed && fixedOnScroll){
            jQuery(window).on('scroll', function(){
                fixedOnScroll(domId);
            });
        }
    }
    var init = function(publisher,platform,zone,domId,size,config){
        var loaded = false;
        var iframe = document.createElement('iframe');
        iframe.id = domId + '-iframe';
        iframe.width = size[0] + 'px';
        iframe.height = size[1] + 'px';
        iframe.frameborder = "0";
        iframe.marginheight = "0";
        iframe.marginwidth = "0";
        iframe.scrolling = "no";
        iframe.style.border = "none";
        iframe.style.width = size[0] + 'px';
        iframe.style.height = size[1] + 'px';
        iframe.style.margin = "0";
        iframe.style.padding = "0";
        iframe.border = "0";
        iframe.frameBorder = "0";
        iframe.allowTransparency = "true";
        
        var pgSSL = 'https:' == window.top.location.protocol;
        var pgEndpoint = (pgSSL ? 'https:' : 'http:') +
        '//delivery.pbgrd.com/network/ad.php' +
        '?publisher=' + publisher + 
        '&platform=' + platform + 
        '&pgzone=' + zone + 
        '&outerheight=' + screen.height + 
        '&outerwidth=' + screen.width + 
        '&cb=' + Math.random();
        if(config.params && config.params.length){
            for(var i=0; i < config.params.length; i++){
                pgEndpoint += '&' + config.params[i][0] + '=' + encodeURIComponent(encodeURIComponent(config.params[i][1]));
            }
        }
        pgEndpoint += '&location=' + encodeURIComponent(encodeURIComponent(window.top.location));
        pgEndpoint += '&referrer=' + encodeURIComponent(encodeURIComponent(document.referrer));
        var html = 'javascript:"<html><body style=\'margin:0;background-color:transparent\'><scr'+'ipt type=\'text/javascript\' src=\'' + pgEndpoint.replace(/\'/g,'').replace(/\"/g,'') + '\'><\/script><img src=\'https://delivery.pbgrd.com/images/?project=biomed&nl=1&asrand=' + Math.random() + '\' width=\'1\' height=\'1\' /></body></html>"';
        iframe.src = html;
        if(config.threshold && visible && !visible(domId,size,config.threshold)){ // check if threshold > 0, if visible is defined and if the zone is not visible
            jQuery(window).on('scroll', function(){
                if(!loaded && visible(domId,size,config.threshold)){
                    show(domId,iframe,config);
                    loaded = true;
                }
            });
        }
        else{
           show(domId,iframe,config);
        }
        if (zone==94) {
            var img = document.createElement('img');
            img.setAttribute('src', 'https://delivery.pbgrd.com/images/?project=biomed&doi=' + (function(){var m=document.getElementsByTagName('meta');for(var i=0;i<m.length;i++){if(m[i].getAttribute('name')=="citation_doi"){return m[i].getAttribute('content');}}return "";})() + '&asrand=' + Math.random());
            img.setAttribute('height', '1');
            img.setAttribute('width', '1');
            //document.body.appendChild(img);
            document.body.insertBefore(img,document.body.firstChild);
        }
    }

    function loadScript(url, callback){
        
        if(typeof jQuery !== 'undefined'){
            callback();
        }
        else {
            var script = document.createElement("script")
            script.type = "text/javascript";
            
            if (script.readyState){
                // IE 
                script.onreadystatechange = function(){
                    if (script.readyState == "loaded" ||
                            script.readyState == "complete"){
                        script.onreadystatechange = null;
                        callback();
                    }
                };
            } else {  
                // all but IE
                script.onload = function(){
                    callback();
                };
            }
        
            script.src = url;
            document.getElementsByTagName("head")[0].appendChild(script);
        }
    } 
    loadScript('//ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js',function(){
        jQuery(document).ready(function(){
           zone.push([2,2,80,'pbgrd-11736203', [728,90],{
               threshold:0,
               params: [['pageid', (function(){var m=document.getElementsByTagName('meta');for(var i=0;i<m.length;i++){if(m[i].getAttribute('name')=="citation_doi"){return m[i].getAttribute('content');}}return "";})() || ''],['journal', (function(){var m=document.getElementsByTagName('meta');for(var i=0;i<m.length;i++){if(m[i].getAttribute('name')=="citation_journal_title"){return m[i].getAttribute('content');}}return "";})() || ''],['sitepage', window.pub_passback_site || '']]
}]);
           zone.push([2,2,94,'pbgrd-43415139', [160,600],{
               threshold:0.5,
               fixed: 1,
               params: [['pageid', (function(){var m=document.getElementsByTagName('meta');for(var i=0;i<m.length;i++){if(m[i].getAttribute('name')=="citation_doi"){return m[i].getAttribute('content');}}return "";})() || ''],['journal', (function(){var m=document.getElementsByTagName('meta');for(var i=0;i<m.length;i++){if(m[i].getAttribute('name')=="citation_journal_title"){return m[i].getAttribute('content');}}return "";})() || '']]
}]);
 
            while(zone.length > 0){
                var z = zone.shift(); //pop the item on top of the stack
                if(jQuery("#" + z[3]).length > 0) { // check if container exists in dom
                    init(z[0], z[1], z[2], z[3], z[4], z[5]);
                }
            }
        });
    });
    
   
var visible = function(domId,size,threshold){ //size = [width,height] array, threshold in [0,1]
    var toscr = jQuery("#" + domId).offset().top - jQuery(window).height();	
    var pgadspace = jQuery(document).height() - jQuery("#" + domId).offset().top;

    var scrpos = (window.pageYOffset || document.body.scrollTop || document.documentElement.scrollTop);
    if (pgadspace > size[1] && scrpos > toscr + size[1] * threshold) {
        return true;
    }
    else{
        return false;
    }
}

/*** FIXES THE ZONE TO THE TOP WHEN SCROLLING */
var fixedOnScroll = function(domID) {
    if(jQuery("#" + domID).offset().top <= jQuery(document).scrollTop() + 20){
        if((/android|webos|iphone|ipad|ipod|blackberry|iemobile|opera mini/i.test(navigator.userAgent.toLowerCase()))){//if mobile device
            jQuery("#" + domID +  "-iframe").css({"position":"absolute","top": (jQuery(document).scrollTop() + 20) + "px"});
        }
        else{
            jQuery("#" + domID +  "-iframe").css({"position":"fixed","top":"20px"});
        }
    }
    else if (jQuery("#" + domID).offset().top >= jQuery(document).scrollTop()){ //we have a "#pbgrdheightpos-" + domID container that we left at the original position of the zone, so we check if we are back there yet!
        jQuery("#" + domID +  "-iframe").css({"position":"static"});
    }
    else if((/android|webos|iphone|ipad|ipod|blackberry|iemobile|opera mini/i.test(navigator.userAgent.toLowerCase()))){//if mobile device we continously set this value as we scroll!
        jQuery("#" + domID +  "-iframe").css({"position":"absolute","top": (jQuery(document).scrollTop() + 20) + "px"});
    }
}
 return {}
})();
