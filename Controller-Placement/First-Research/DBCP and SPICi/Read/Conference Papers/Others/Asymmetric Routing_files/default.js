/*Variables for functions*/
var images;
var allNews;
var masonryObjects;
/*Randomizer for wordcloud/tagcloud*/
function random_sort(thing)
{
      return (0.5 - Math.random() );
}

$("document").ready(function(){
	
	/*Search box focus and blur settings*/
	var el = $('#search_text');
    el.focus(function(e) {
		el.css("color","#000");
		el.css("textTransform","none");
        if (e.target.value == e.target.defaultValue)
			e.target.value = '';
    });
    el.blur(function(e) {
        if (e.target.value == '')
			el.css("color","#afafaf");
			el.css("textTransform","uppercase");
			e.target.value = e.target.defaultValue;
    });
	
	/*Check if the page is the frontpage*/
	if($(".frontpage").hasClass("mainsitefront")){
		getNews();
		getWeather();
		getSlides();
		newsSlider();
		fpImagesClick();
	}
	/*Check if the page is the solutions-frontpage*/
	if($(".frontpage").hasClass("solutions")){
		getMasonryObjects();
		masonryClickNHover();
	}
	/*Check if the page is the products-frontpage*/
	if($(".frontpage").hasClass("products")){
		
		var obj = $('#tags ul li');
		var arr = $.makeArray(obj);
		arr.sort(random_sort);
		$('#tags ul').empty();
		$(arr).appendTo('#tags ul');
	}
	/*Effects and events for main menu*/
	menuHovers();
	
});




/*
GET NEWS&WEATHER FOR FRONTPAGE

Currently uses ajax to read JSON files that have newsitems in them, can be changed to read xml files or printing them just out on the frontpage and usin visibility to trigger them.
The weather uses a php file that calls the Yahoo-weather api and gets a JSON-response which it then relays to this script 
*/
function getWeather(){
	$.getJSON('/opencms/opencms/system/modules/com.clicksandlinks.opencms.sites.stonesoft/files/weather.json', function(data) {
		  $("#temperature h2 #temp_val").prepend(data.condition.temperature);
		  $("#temperature h2 #temp_val").animate({marginTop:0},240,"linear",function(){ });
	 });
}
function getNews(){
	
	var root_uri = location.href;
	root_uri = root_uri.substring(root_uri.indexOf(document.domain)+document.domain.length, root_uri.length);
	if(root_uri.indexOf("/opencms/opencms/")==0)
		root_uri = root_uri.substring("/opencms/opencms/".length-1, root_uri.length);
	if(root_uri.indexOf("/opencms/export/")==0)
		root_uri = root_uri.substring("/opencms/export/".length-1, root_uri.length);
	if(root_uri.indexOf("/",2)>0 && root_uri.indexOf("/",2)<root_uri.length)
		root_uri = root_uri.substring(0, root_uri.indexOf("/",2)+1);
	$.ajax({
		url: '/opencms/opencms/system/modules/com.clicksandlinks.opencms.sites.stonesoft/files/news.json?root_uri='+root_uri,
	    dataType: 'text',
	    success: function(result){
	    	storeNews(JSON.parse(result));
	    }, 
	    error: function(xhr) {
	        //alert (xhr.statusText);
	    }
	});
}
function storeNews(ns){
	allNews=ns;
	f=parseInt($("#dates .date ").length)-1;
	i=0;
	$("#news a").attr("href",allNews.news[f].newsItem.href);
	$("#news p").append(allNews.news[i].newsItem.blurb);
	$("#dates .date a").each(function(){
		$(this).empty();
		$(this).append(allNews.news[i].newsItem.date);
		i++;
	});
}
/*The sliding balls for the newsslider*/
function newsSlider(){
	$(".date a").click(function(){
		$(".date").removeClass("active");
		var nr=parseInt($(this).attr("href").substr(1,1))-1;
		$(this).parent(".date").addClass("active");
		$("#news p").fadeOut(240,"linear",function(){
			$(this).empty();
			$("#news a").attr("href",allNews.news[nr].newsItem.href);
			$("#news p").append(allNews.news[nr].newsItem.blurb);
			$("#news p").fadeIn(240,"linear",function(){});
		});
		$("#news a").fadeOut(240,"linear",function(){$("#news a").fadeIn(240,"linear",function(){ });});
		$("#time_slider").animate({marginLeft:(((nr)*67)+((nr)*59)+10)},240,"linear",function(){ });
	});
}


/*
MASONRY FUNCTIONS

Get objects that have the masonry effect from a json file and print them on he page, can probably be changed to do dynamically in the CMS animate them when clicked.
*/
function getMasonryObjects(){
	$.getJSON('/opencms/opencms/system/modules/com.clicksandlinks.opencms.sites.stonesoft/files/masonry_objects.json', function(data) {
	  printMasonryObjects(data);
	});
}
function printMasonryObjects(obs){
	masonryObjects=obs;
	
	for(i=0;i<obs.masonryItems.length;i++){
		bg_color=obs.masonryItems[i].item.background;
		title=obs.masonryItems[i].item.title;
		text=obs.masonryItems[i].item.text;
		link_url=obs.masonryItems[i].item.linkUrl
		link_text=obs.masonryItems[i].item.linkTxt
		$("#isotope_container").append("<div style='background:"+bg_color+";' num='"+i+"' class='i_item closed'><h3>"+title+"</h3><p>"+text+"</p><a href='"+link_url+"'>"+link_text+"<div class='arrow-right'></div></a><div class='arrow-right-white'></div></div>");
	}
	$('#isotope_container').isotope({
    // options
    itemSelector : '.i_item',
    layoutMode : 'masonry',
    animationOptions: {
		duration: 400,
		easing: 'linear',
		queue: false
    },
	resizable: false,
	masonry: {
		columnWidth: 75
	}
	});
	relayOut();
	masonryClickNHover();
}
function relayOut(){
	$('#isotope_container').isotope("reLayout");
}
function masonryClickNHover(){
	$("#isotope_container .i_item").hover(function(){
		num=$(this).attr('num');
		if(!$(this).hasClass("open")){
		$(this).css({'background-image' : 'url(http://'+window.location.host+"/img/"+masonryObjects.masonryItems[num].item.hover+')','background-repeat': 'no-repeat'});
		}
	},function(){
		num=$(this).attr('num');
		if(!$(this).hasClass("open")){
		$(this).css("backgroundImage","none");
		}
	});
	$("#isotope_container .i_item").click(function(){
	
		$("#isotope_container .i_item").each(function(){ 
			if($(this).hasClass("open")){
				$(this).css("backgroundImage","none");
				$(this).animate({width:180,height:95},200);
			}
			$(this).removeClass("open");
			$(this).addClass("closed");
			});
	$(this).removeClass("closed");
	$(this).animate({width:405,height: 230},{duration:400,step:function(){num=$(this).attr('num'); $(this).addClass("open");$(this).css({'background-image' : 'url(http://'+window.location.host+"/img/"+masonryObjects.masonryItems[num].item.bg+')','background-repeat': 'no-repeat'});$('#isotope_container').isotope("reLayout");}});
	
});
}

/*
MENUS AND THEIR ANIMATIONS
*/
function menuHovers(){

	$("#site_nav_toggle_open").click(function(){$("#toggle_nav").animate({top:-1},200);});
	$("#site_nav_toggle_close").click(function(){$("#toggle_nav").animate({top:-320},200);});

	$("#home_link").hover(function(){
		$("#main_navigation>li").removeClass("active");
		$("#main_navigation>li .arrow-up").remove();
		$("#home_link .arrow_up").remove();
		$("#home_link").append("<div class='arrow-up'></div>");
		$("#home_link").addClass("active");
	},function(){
		if(!$("#home_link").hasClass("active-page")){
		$("#home_link .arrow-up").remove();
		$("#home_link").removeClass("active");
		$(".active-page").append("<div class='arrow-up'></div>");
	}
	});
	
	var _timer_header_toggle;

	if($(".frontpage").hasClass("mainsitefront")){
		
		$("#main_navigation").hover(function(){		
			$("#home_link").removeClass("active");
			$("#home_link .arrow-up").remove();	
			clearTimeout(_timer_header_toggle);
			$("#fp_sidebar").stop().animate({top:206}, 200, "linear");
			
			$("header").stop().animate({height:164}, 200, "linear", function(){
				$("header").css({"overflow":"visible"});
			});
		},
		function(){			
			_timer_header_toggle = setTimeout(function(){
				$("#fp_sidebar").stop().delay(75).animate({top:165}, 200, "linear");
				$("header").stop().delay(75).animate({height:123}, 200, "linear", function(){
					$("header").css({"overflow":"hidden"});
					$("#main_navigation>li").removeClass("active");
					$("#home_link").append("<div class='arrow-up'></div>");
					$("#home_link").addClass("active");					
				});
			}, 800);
					
		});
	}
	else{
		$("header").css("overflow","visible");
//		$("header").css("height","164px"); 
	}
	$("#main_navigation>li").hover(function(){
		$("#main_navigation>li").removeClass("active");
		$("#main_navigation>li").removeClass("last-visible");
		$("#main_navigation>li .arrow-up").remove();
		$(this).append("<div class='arrow-up'></div>");
		$(this).addClass("active");	
	},function(){
		$(this).addClass("last-visible");
		$(this).removeClass("active");
		$(this).children(".arrow-up").remove();
		$(".active-page").append("<div class='arrow-up'></div>");
	});
	$(".sub_nav_wrap").hover(function(){	
		$(this).siblings("a").css("color","#e63e97");
	},function(){
		$(this).siblings("a").css("color","");
	});
		
	
}
/*
FRONTPAGE LARGE IMAGE FADE
Gets images locations from a JSON file
*/
function getSlides(){
	var currentPage = location.href;
	currentPage = currentPage.substring(currentPage.indexOf(document.domain)+document.domain.length, currentPage.length);
	if(currentPage.indexOf("/opencms/opencms/")==0)
		currentPage = currentPage.substring("/opencms/opencms/".length-1, currentPage.length);
	if(currentPage.indexOf("/opencms/export/")==0)
		currentPage = currentPage.substring("/opencms/export/".length-1, currentPage.length);
	if(currentPage.indexOf("index.html")==-1)
		currentPage = currentPage + "index.html";
	$.ajax({
	    url: '/opencms/opencms/system/modules/com.clicksandlinks.opencms.sites.stonesoft/files/front_images.json?page_uri='+currentPage,
	    dataType: 'text',
	    success: function(result){
	    	storeImages(JSON.parse(result));
	    }, 
	    error: function(xhr) {
	        //alert (xhr.statusText);
	    }
	});
}
function storeImages(imgs){
	images=imgs;
	setFirstImage();
}
/*Sets first image from json file, there is a fallback in css*/
function setFirstImage(){
	$("#fp_images .image_text").css("backgroundImage","url('"+images.imagesFront[0].img.text_img+"')");
	$("#fp_images  #big_image").css("backgroundImage","url('"+images.imagesFront[0].img.img_url+"')");
	$("#fp_images .read_more_link a").attr("href",images.imagesFront[0].img.href);
}
/*FRONTPAGE IMAGE CHANGE
When link element is clicked changes image in image carousel and textimage and href value for read more link
*/
function fpImagesClick(){
	
	$("#fp_images .links_and_progressbar a").click(function(){	
		
		$("#fp_images .read_more_link").stop();
		$(".image_text").stop();
		$("#fp_images #big_image").stop();
		
		var nr=parseInt($(this).attr("href").substr(1,1));
		image=nr-1;
		
		$("#fp_images #big_image_bg_fade").css("backgroundImage","url('"+images.imagesFront[image].img.img_url+"')");
		
		$("#fp_images .links_and_progressbar a").each(function(){
				$(this).removeClass("active");
			});
		$(this).addClass("active");
		
		$("#fp_images .read_more_link a").attr("href",images.imagesFront[image].img.href);
	
		
		$("#fp_images .read_more_link a").attr("href",images.imagesFront[image].img.href);
		
		$("#fp_images .read_more_link").animate({opacity:0},400,"linear",function(){
				$("#fp_images .read_more_link a").attr("href",images.imagesFront[image].img.href);
				$("#fp_images .read_more_link").animate({opacity:1},400,"linear");
		});
		
		$(".image_text").animate({opacity:0},400,"linear",function(){
				$("#fp_images .image_text").css("backgroundImage","url('"+images.imagesFront[image].img.text_img+"')"); 
				$("#fp_images .image_text").animate({opacity:1},400,"linear");
		});
		
		$("#fp_images #big_image").animate({opacity:0},650,"linear",function(){
			$("#fp_images #big_image").css("backgroundImage","url('"+images.imagesFront[image].img.img_url+"')");
			$("#fp_images #big_image").css("opacity","1")
		});
	});
}