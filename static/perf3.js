$(function(){
    // var cpuChart = echarts.init(document.getElementById('CPU'));
    var charts = {};
    charts['RAM'] = echarts.init(document.getElementById('RAM'));
    charts['DISK'] = echarts.init(document.getElementById('DISK'));
    charts['NET'] = echarts.init(document.getElementById('NET'));

    var elements = {};
    elements['RAM'] = document.getElementById('RAM');
    elements['DISK'] = document.getElementById('DISK');
    elements['NET'] = document.getElementById('NET');

    var options_tmp = {};
    var options = {};

    function sortNumber(a, b) {
        var a1 = Number(a.match(/cpu(\d*)/)[1]);
        var b1 = Number(b.match(/cpu(\d*)/)[1]);
        return a1 - b1;
    }

    window.onresize = function() {
        console.log('onresize');
        var keys = Object.keys(charts);
        for (var i = 0, key; key = keys[i]; i++) {
            charts[key].resize();
        }
    }

    function visible(element) {
        var top = $(window).scrollTop();
        var bottom = $(window).scrollTop() + $(window).height();
        if (isScrolling) {
            return false;
        }
        if ((element.offsetTop + element.offsetHeight >= top && element.offsetTop + element.offsetHeight <= bottom) || (element.offsetTop >= top && element.offsetTop <= bottom)) {
            return true;
        }
        else {
            return false;
        }
    }

    function setCpu(key, data) {
        var option = {
            title: {
                text: key,
                textStyle: {
                    fontSize: 30,
                }
            },
            tooltip: {},
            legend: {
                data:[
                        {name:'steal', icon:'bar'},  //, textStyle: {color: 'rgb(51, 102, 204)'}
                        {name:'softirq', icon:'bar'},  //, textStyle: {color: 'rgb(214, 99, 0)'}
                        {name:'user', icon:'bar'},  //, textStyle: {color: 'rgb(221, 221, 0)'}
                        {name:'system', icon:'bar'},  //, textStyle: {color: 'rgb(59, 62, 172)'}
                        {name:'nice', icon:'bar'},  //, textStyle: {color: 'rgb(238, 153, 17)'}
                        {name:'iowait', icon:'bar'}  //, textStyle: {color: 'rgb(187, 68, 204)'}
                    ],
                orient:'vertical',
                x:'right',
                top: '30%',
            },
            grid: {
                x:'5%',
                x2:'10%',
            },
            toolbox: {
                show: true,
                feature: {
                    saveAsImage: {show: true}
                }
            },
            xAxis: {
                type: 'category',
                boundaryGap: false,
                splitLine: {show: false},
                data: data.xAxis
            },
            yAxis: {
                name: ' %',
                type: 'value',
                splitLine: {show: false},
                boundaryGap: [0, '60%'],
                max: 100,
            },
            tooltip: {
                trigger: 'axis',
                position: function (pt) {
                    return [pt[0], '10%'];
                }
            },
            series: [{
                name: 'steal',
                type: 'line',
                symbol: 'none',
                stack: 'cpu',
                smooth: true,
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.steal
            },
            {
                name: 'softirq',
                type: 'line',
                symbol: 'none',
                stack: 'cpu',
                smooth: true,
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.softirq
            },
            {
                name: 'user',
                type: 'line',
                symbol: 'none',
                stack: 'cpu',
                smooth: true,
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.user
            },
            {
                name: 'system',
                type: 'line',
                symbol: 'none',
                stack: 'cpu',
                smooth: true,
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.system
            },
            {
                name: 'nice',
                type: 'line',
                symbol: 'none',
                stack: 'cpu',
                smooth: true,
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.nice
            },
            {
                name: 'iowait',
                type: 'line',
                symbol: 'none',
                stack: 'cpu',
                smooth: true,
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.iowait
            }]
        };
        // console.log(div);
        // console.log(option);
        
        options[key] = option;
        if (visible(elements[key])) {
            options_tmp[key] = option;
            charts[key].setOption(option);
        }
    }

    function setCpus(data) {
        // console.log(data)
        var keys = Object.keys(data.data).sort(sortNumber);
        if ($("#CPUs").children().length == 0) {
            for (var i = 0, key; key = keys[i]; i++) {
                var cpu_div = $('<div></div>')
                cpu_div.attr('id', key);
                cpu_div.height(320);
                $("#CPUs").append(cpu_div);
                // console.log(cpu_div[0]);
                elements[key] = cpu_div[0];
                charts[key] = echarts.init(cpu_div[0]);
            }
        }
        for (var i = 0, key; key = keys[i]; i++) {
            // var cpu = $("#"+key)
            // console.log(cpu);
            var cpu_data = {xAxis:data.xAxis, data:data.data[key]};
            setCpu(key, cpu_data);
        }
        
    }

    function setRam(data) {
        var option = {
            title: {
                text: 'RAM',
                textStyle: {
                    fontSize: 30,
                }
            },
            tooltip: {},
            legend: {
                data:['buffers', 'cached', 'memFree', 'memTotal', 'swapFree', 'swapTotal'],
                orient:'vertical',
                x:'right',
                top: '30%',
            },
            grid: {
                x:'5%',
                x2:'10%',
            },
            toolbox: {
                show: true,
                feature: {
                    saveAsImage: {show: true}
                }
            },
            xAxis: {
                type: 'category',
                boundaryGap: false,
                splitLine: {show: false},
                data: data.xAxis
            },
            yAxis: {
                name: 'MB',
                type: 'value',
                splitLine: {show: false},
                boundaryGap: [0, '60%']
            },
            tooltip: {
                trigger: 'axis',
                position: function (pt) {
                    return [pt[0], '10%'];
                }
            },
            series: [{
                name: 'buffers',
                type: 'line',
                symbol: 'none',
                stack: 'mem',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.buffers
            },
            {
                name: 'cached',
                type: 'line',
                symbol: 'none',
                stack: 'mem',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.cached
            },
            {
                name: 'memFree',
                type: 'line',
                symbol: 'none',
                stack: 'mem',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.memFree
            },
            {
                name: 'memTotal',
                type: 'line',
                symbol: 'none',
                stack: 'mem',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.memTotal
            },
            {
                name: 'swapFree',
                type: 'line',
                symbol: 'none',
                stack: 'mem',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.swapFree
            },
            {
                name: 'swapTotal',
                type: 'line',
                symbol: 'none',
                stack: 'mem',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.swapTotal
            }]
        };
        
        options['RAM'] = option;
        if (visible(elements['RAM'])) {
            options_tmp['RAM'] = option;
            charts['RAM'].setOption(option);
        }
    }

    function setDisk(data) {
        var option = {
            title: {
                text: 'DISK',
                textStyle: {
                    fontSize: 30,
                }
            },
            tooltip: {},
            legend: {
                data:['read', 'write'],
                orient:'vertical',
                x:'right',
                top: '30%',
            },
            toolbox: {
                show: true,
                feature: {
                    saveAsImage: {show: true}
                }
            },
            grid: {
                x:'5%',
                x2:'10%',
            },
            xAxis: {
                type: 'category',
                boundaryGap: false,
                splitLine: {show: false},
                data: data.xAxis
            },
            yAxis: {
                name: 'KB/s',
                type: 'value',
                boundaryGap: [0, '60%'],
                splitLine: {show: false},
            },
            tooltip: {
                trigger: 'axis',
                position: function (pt) {
                    return [pt[0], '10%'];
                }
            },
            series: [{
                name: 'read',
                type: 'line',
                symbol: 'none',
                smooth: true,
                stack: 'disk',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.read
            },
            {
                name: 'write',
                type: 'line',
                symbol: 'none',
                smooth: true,
                stack: 'disk',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.write
            }]
        };
        options['DISK'] = option;
        if (visible(elements['DISK'])) {
            options_tmp['DISK'] = option;
            charts['DISK'].setOption(option);
        }
    }

    function setNet(data) {
        var option = {
            title: {
                text: 'NET',
                textStyle: {
                    fontSize: 30,
                }
            },
            tooltip: {},
            legend: {
                data:['send', 'receive'],
                orient:'vertical',
                x:'right',
                top: '30%',
            },
            toolbox: {
                show: true,
                feature: {
                    saveAsImage: {show: true}
                }
            },
            grid: {
                x:'5%',
                x2:'10%',
            },
            xAxis: {
                type: 'category',
                boundaryGap: false,
                splitLine: {show: false},
                data: data.xAxis
            },
            yAxis: {
                name: 'KB/s',
                type: 'value',
                boundaryGap: [0, '60%'],
                splitLine: {show: false},
            },
            tooltip: {
                trigger: 'axis',
                position: function (pt) {
                    return [pt[0], '10%'];
                }
            },
            series: [{
                name: 'receive',
                type: 'line',
                symbol: 'none',
                smooth: true,
                stack: 'net',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.receive
            },
            {
                name: 'send',
                type: 'line',
                symbol: 'none',
                smooth: true,
                stack: 'net',
                areaStyle: {normal: {}},
                lineStyle: {
                    normal: {
                        width: 0
                    }
                },
                data: data.data.send
            }]
        };
        options['NET'] = option;
        if (visible(elements['NET'])) {
            options_tmp['NET'] = option;
            charts['NET'].setOption(option);
        }
    }

    var socket;
    socket = new WebSocket('ws://' + window.location.hostname + ":" + (parseInt(window.location.port) - 1));
    socket.onopen = function(event) {
        // console.log("onopen")
    }

    var cpu_data = {xAxis:[], data:{}}
    var mem_data = {xAxis:[], data:{buffers:[], cached:[], memFree:[], memTotal:[], swapFree:[], swapTotal:[]}};
    var disk_data = {xAxis:[], data:{read:[], write:[]}};
    var net_data = {xAxis:[], data:{receive:[], send:[]}};

    function dataToString(date){
        var dateStr = date.getFullYear() + '-' + ('0' + (date.getMonth() + 1)).slice(-2) + '-' + ('0' + date.getDate()).slice(-2) + ' ' + ('0' + date.getHours()).slice(-2) + ':' + ('0' + date.getMinutes()).slice(-2) + ':' + ('0' + date.getSeconds()).slice(-2);
        return dateStr;
    }

    var size = 60 * 5;

    socket.onmessage = function(event) {
        data = JSON.parse(event.data)
        // console.log(data);
        if(data.type == 'cpu') {
            cpu_data.xAxis.push(dataToString(new Date(parseInt(data.time) * 1000)));
            if(cpu_data.xAxis.length > size) {
                cpu_data.xAxis.shift();
            }
            for (var key in data) {
                if(key.startsWith('cpu')) {
                    if(!cpu_data.data[key]) {
                        cpu_data.data[key] = []
                    }
                    for(var cpu_key in data[key]) {
                        if(!cpu_data.data[key][cpu_key]) {
                            cpu_data.data[key][cpu_key] = []
                        }
                        cpu_data.data[key][cpu_key].push(data[key][cpu_key])
                        if(cpu_data.data[key][cpu_key].length > size) {
                            cpu_data.data[key][cpu_key].shift();
                        }
                    }
                }
            }
            setCpus(cpu_data);
        } else if(data.type == 'mem') {
            mem_data.xAxis.push(dataToString(new Date(parseInt(data.time) * 1000)));
            mem_data.data.buffers.push(Math.round(parseInt(data.buffers)/1024));
            mem_data.data.cached.push(Math.round(parseInt(data.cached)/1024));
            mem_data.data.memFree.push(Math.round(parseInt(data.memFree)/1024));
            mem_data.data.memTotal.push(Math.round(parseInt(data.memTotal)/1024));
            mem_data.data.swapFree.push(Math.round(parseInt(data.swapFree)/1024));
            mem_data.data.swapTotal.push(Math.round(parseInt(data.swapTotal)/1024));
            if(mem_data.xAxis.length > size) {
                mem_data.xAxis.shift();
                mem_data.data.buffers.shift();
                mem_data.data.cached.shift();
                mem_data.data.memFree.shift();
                mem_data.data.memTotal.shift();
                mem_data.data.swapFree.shift();
                mem_data.data.swapTotal.shift();
            }
            setRam(mem_data);
        } else if(data.type == 'disk') {
            disk_data.xAxis.push(dataToString(new Date(parseInt(data.time) * 1000)));
            disk_data.data.read.push(Math.round(parseInt(data.read)/1024));
            disk_data.data.write.push(Math.round(parseInt(data.write)/1024));
            if(disk_data.xAxis.length > size) {
                disk_data.xAxis.shift();
                disk_data.data.read.shift();
                disk_data.data.write.shift();
            }
            setDisk(disk_data);
        } else if(data.type == 'net') {
            net_data.xAxis.push(dataToString(new Date(parseInt(data.time) * 1000)));
            net_data.data.receive.push(Math.round(parseInt(data.receive)/1024));
            net_data.data.send.push(Math.round(parseInt(data.send)/1024));
            if(net_data.xAxis.length > size) {
                net_data.xAxis.shift();
                net_data.data.receive.shift();
                net_data.data.send.shift();
            }
            setNet(net_data);
        } else if (data.type == 'traffic') {
            var array = [];
            for (key in data) {
                array.push({"ip": key, "speed_in": (data[key]["speed_in"] / 1024).toFixed(2), "speed_out": (data[key]["speed_out"] / 1024).toFixed(2),
                    "total_in": (data[key]["total_in"] / 1024).toFixed(0), "total_out": (data[key]["total_out"] / 1024).toFixed(0)});
                
            }
            array.splice(-1, 1);

            $(".table").removeClass('hide');
            trafficTable.setData(array);

        } else if (data.type == 'processes') {
            var list = []
            for(var key in data.data) {
                var v = data.data[key];
                for(var k in v) {
                    if (k == 'cpu') {
                        v[k] = (v[k] * 100).toFixed(2);
                    } else if (k == 'mem') {
                        v[k] = (v[k] / 1024 / 1024).toFixed(0);
                    } else {
                        v[k] = v[k]
                    }
                }
                list.push(v)
            }
            processTable.setData(list);
        }

    }

    var processTable = new SortTable({
        container:$('#process_table'),
        column:{'pid':'int', 'cpu':'float', 'mem':'int', 'task_cpu':'int', 'cmdline':'string'},
        max_row:20,
        default_sort:1,
        desc:false,
    });

    var trafficTable = new SortTable({
        container:$('#traffic_table'),
        column:{'ip':'string', 'speed_in':'float', 'speed_out':'float', 'total_in':'int', 'total_out':'int'},
        max_row:20,
        default_sort:1,
        desc:false,
    });


    socket.onclose = function(event) {
        // console.log("onclose")
    }
   

    function set_cur(key) {
        var lis = $('#J_navMenu').find("a");
        // console.log(lis);
        lis.removeClass('active');
        // console.log(lis);
        // console.log(key);
        var ele = $("a[href='#" + key + "']");
        ele.addClass('active');
    }

    var topValue = 0, interval = null, isScrolling = false;
    $(window).scroll(function() {
        var cpus_top = $('#CPUs').offset().top;
        var ram_top = $('#RAM').offset().top;
        var disk_top = $('#DISK').offset().top;
        var net_top = $('#NET').offset().top;

        var scrollH = $(this).scrollTop();

        if (scrollH >= net_top) {
            set_cur('NET');
        } else if (scrollH >= disk_top) {
            set_cur('DISK');
        } else if (scrollH >= ram_top) {
            set_cur('RAM');
        } else if (scrollH >= cpus_top) {
            set_cur('CPUs');
        }

        showCharts();

        if(interval == null) {
            interval = setInterval(scrolling, 1000);
            isScrolling = true;
        }
        topValue = $(this).scrollTop();
    });

    function showCharts() {
        var keys = Object.keys(elements);
        for (var i = 0, key; key = keys[i]; i++) {
            var element = elements[key];
            var chart = charts[key];
            if (visible(element)) {
                option = options[key];
                option_tmp = options_tmp[key];
                if (option_tmp != option) {
                    charts[key].setOption(option);
                    options_tmp[key] = option;
                }
            }
        }
    }

    function scrolling() {
        if($(this).scrollTop() == topValue) {
            // console.log("scrolling stop");
            isScrolling = false;
            showCharts();
            clearInterval(interval);
            interval = null;
        }
    }

    $("a[href^='#']").click(function(e) {
        e.preventDefault();
        var dest = $(this).attr('href');
        $('#J_navMenu').addClass('animating');
        $('html,body').animate({
            scrollTop: $(dest).offset().top
        }, 'slow', function() {
            $('#J_navMenu').removeClass('animating');
        });
    });


    var toTop = {
        init: function() {
            $('#toTop').click(function() {
                $("html, body").animate({
                    scrollTop: 0
                }, 120);
            });
            $(window).bind("scroll", this.backToTopFun);
            this.backToTopFun();
        },
        backToTopFun: function() {
            var st = $(document).scrollTop(),
            winh = $(window).height();
            var $toTop = $('#toTop');

            (st > winh) ? $toTop.show(): $toTop.hide();
        }
    };
    toTop.init();

});




