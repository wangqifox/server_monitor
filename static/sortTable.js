;(function(util) {
    function SortTable(options) {
        options = options || {};
        this.container = options.container[0];
        this.column = options.column;
        this.ths = this.container.querySelectorAll('th');
        this.tbody = this.container.querySelector('tbody');

        this.default_sort = options.default_sort
        this._sortby = this.ths[this.default_sort].getAttribute('class');
        this._desc = options.desc;

        util.extend(this, options);
        this._initEvent();
    }

    util.extend(SortTable.prototype, {

        _initEvent: function() {
            for (var i = 0, th; th = this.ths[i]; i++){
                th.addEventListener(
                    'click', this._sort.bind(this, this.ths[i])
                );
            }
            this._sort.bind(this, this.ths[this.default_sort])();
        },

        _sort: function(element) {
            var attr = element.getAttribute('class');

            if (this._sortby == attr) {
                this._desc = !this._desc;
                var spans = element.getElementsByTagName('span');
                for(var i = 0, span; span = spans[i]; i++)
                    element.removeChild(span);

                element.innerHTML = '<span>' + (this._desc ? "&#9662;" : "&#9652;") + '</span>' + element.innerHTML;
            } else {
                var th = this.container.querySelector('.'+this._sortby);
                var spans = th.getElementsByTagName('span');
                for(var i = 0, span; span = spans[i]; i++)
                    th.removeChild(span);

                this._sortby = attr;
                this._desc = false;

                element.innerHTML = '<span>' + (this._desc ? "&#9662;" : "&#9652;") + '</span>' + element.innerHTML;
            }

        },

        setData: function(data) {
            var myHtml = "";
            data.sort(compare.bind(this));
            for(var i = 0, v; v = data[i]; i++) {
                if (i > this.max_row) break;
                myHtml += "<tr>";
                for(var key in this.column) {
                    myHtml += "<td>" + v[key] + "</td>";
                }
                myHtml += "</tr>";
            }
            this.tbody.innerHTML = myHtml;

        }
    });

    function compare(a1, a2) {
        var v1 = a1[this._sortby];
        var v2 = a2[this._sortby];
        if (this.column[this._sortby] == 'int') {
            v1 = parseInt(v1);
            v2 = parseInt(v2);
        } else if (this.column[this._sortby] == 'float') {
            v1 = parseFloat(v1);
            v2 = parseFloat(v2);
        }

        var ret = v1 < v2 ? -1 : 1;
        if(this._desc)
            ret *= -1;
        return ret;
    }

    util.extend(SortTable.prototype, util.emitter);

    window.SortTable = SortTable;

})(util);