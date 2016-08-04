$(function(){
    var cpuChart = echarts.init(document.getElementById('CPU'));
    var ramChart = echarts.init(document.getElementById('RAM'));
    var diskChart = echarts.init(document.getElementById('DISK'));
    var netChart = echarts.init(document.getElementById('NET'));

    function getCpuOption(data) {
        var series = []
        var grid = []
        var xAxis = []
        var yAxis = []
        var legend = []
        
        var keys = Object.keys(data.data).sort()

        for(var i = 0, k; k = keys[i]; i++) {
            for(var j = 0, key; key = Object.keys(data.data[k]).sort()[j]; j++) {
                var v = {
                    name: key,
                    type: 'line',
                    xAxisIndex: i,
                    yAxisIndex: i,
                    symbol: 'none',
                    smooth: true,
                    stack: k,
                    lineStyle: {
                        normal: {
                            width: 0
                        }
                    },
                    data: data.data[k][key]
                }
                switch(key) {
                    case 'steal':
                        v.itemStyle = {normal: {
                            color: '#91c7ae',
                            areaStyle: {
                                color: '#91c7ae',
                            }
                        }}
                        series.push(v)
                        break
                    case 'softirq':
                        v.itemStyle = {normal: {
                            color: '#d48265',
                            areaStyle: {
                                color: '#d48265',
                            }
                        }}
                        series.push(v)
                        break
                    case 'user':
                        v.itemStyle = {normal: {
                            color: '#2f4554',
                            areaStyle: {
                                color: '#2f4554',
                            }
                        }}
                        series.push(v)
                        break
                    case 'system':
                        v.itemStyle = {normal: {
                            color: '#c23531',
                            areaStyle: {
                                color: '#c23531'
                            }
                        }}
                        series.push(v)
                        break
                    case 'nice':
                        v.itemStyle = {normal: {
                            color: '#61a0a8',
                            areaStyle: {
                                color: '#61a0a8',
                            }
                        }}
                        series.push(v)
                        break
                    case 'iowait':
                        v.itemStyle = {normal: {
                            color: '#749f83',
                            areaStyle: {
                                color: '#749f83',
                            }
                        }}
                        series.push(v)
                }
                
            }
        }

        for(var i = 0, k; k = keys[i]; i++) {
            grid.push({
                top: i * (100 / keys.length) + 6 + '%',
                height: '10%',
                x:'5%',
                x2:'10%',
            })

            xAxis.push({
                gridIndex: i,
                type: 'category',
                axisLine: {onZero: true},
                splitLine: {show: false},
                boundaryGap: false,
                data: data.xAxis
            })

            yAxis.push({
                gridIndex: i,
                name: k + ' %',
                type: 'value',
                splitLine: {show: false},
            })

            legend.push({
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
                top: i * (100 / keys.length) + 6 + '%',
                index: i,
                
            })
        }

        var option = {
            title: {
                text: 'CPU',
                // x: 'center'
                textStyle: {
                    fontSize: 30,
                }
            },
            legend: legend,
            toolbox: {
                show: true,
                feature: {
                    saveAsImage: {show: true}
                }
            },
            tooltip: {
                 trigger: 'axis',
                axisPointer: {
                    animation: false
                }
            },
            grid: grid,
            xAxis: xAxis,
            yAxis: yAxis,
            series: series
        }
        return option

    }

    function setCpu(data) {
        var option = getCpuOption(data);
        cpuChart.setOption(option);
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

        ramChart.setOption(option);
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

        diskChart.setOption(option);
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

        netChart.setOption(option);
    }

    var socket;
    socket = new WebSocket('ws://' + window.location.hostname + ":" + (parseInt(window.location.port) - 1));
    socket.onopen = function(event) {
        console.log("onopen")
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
            console.log(cpu_data);
            $("#CPU").height(250 * Object.keys(cpu_data.data).length);
            cpuChart.resize();
            setCpu(cpu_data);
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
        }

    }

    socket.onclose = function(event) {
        console.log("onclose")
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


