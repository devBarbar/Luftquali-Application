
var chartColors = {
  red: 'rgb(255, 99, 132)',
  orange: 'rgb(255, 159, 64)',
  yellow: 'rgb(255, 205, 86)',
  green: 'rgb(75, 192, 192)',
  info: '#41B1F9',
  blue: '#3245D1',
  purple: 'rgb(153, 102, 255)',
  grey: '#EBEFF6'
};

var config_CO2 = { //This is the Config for one of the CO2 Graphs
  type: "line",
  data: {
    labels: ["VAR:CO2_GRAPH_OLD_6", "VAR:CO2_GRAPH_OLD_5", "VAR:CO2_GRAPH_OLD_4", "VAR:CO2_GRAPH_OLD_3", "VAR:CO2_GRAPH_OLD_2", "VAR:CO2_GRAPH_OLD_1", "VAR:CO2_GRAPH_CURRENT"], //EXTEND HOW MANY LABLES ARE DISPLAYED (MAYBE inside Settings)
    datasets: [
      {
        label: "CO2_MG811",
        backgroundColor: "#0f0",
        borderColor: "#0f0",
        data: [20, 40, 20, 70, 10, 50, 100],  //TODO HERE TO PUT THE LIVE VARIABLES
        fill: false,
        pointBorderWidth: 100,
        pointBorderColor: "transparent",
        pointRadius: 3,
        pointBackgroundColor: "transparent",
        pointHoverBackgroundColor: "rgba(63,82,227,1)",
      },
      {
        label: "CO2_SHT21",
        backgroundColor: "#f00",
        borderColor: "#f00",
        data: [40, 30, 80, 50, 30, 70, 10],  //TODO HERE TO PUT THE LIVE VARIABLES
        fill: false,
        pointBorderWidth: 100,
        pointBorderColor: "transparent",
        pointRadius: 3,
        pointBackgroundColor: "transparent",
        pointHoverBackgroundColor: "rgba(63,82,227,1)",
      },
    ],
  },
  options: {
    responsive: true,
    maintainAspectRatio: false, 
    layout: {
      padding: {
        left: -10,  //Damit die line auch genau am rand beginnt
        top: 10,
      },
    },
    legend: {
      display: true,
    },
    title: {
      display: false,
    },
    tooltips: {
      mode: "index",
      intersect: false,
    },
    hover: {
      mode: "nearest",
      intersect: true,
    },
    scales: {
      xAxes: [
        {
          gridLines: {
            drawBorder: false,
            display: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
      yAxes: [
        {
          gridLines: {
            display: false,
            drawBorder: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
    },
  },
};
var config_volatiles = {
  type: "line",
  data: {
    labels: ["VAR:VOLATILES_GRAPH_OLD_6", "VAR:VOLATILES_GRAPH_OLD_5", "VAR:VOLATILES_GRAPH_OLD_4", "VAR:VOLATILES_GRAPH_OLD_3", "VAR:VOLATILES_GRAPH_OLD_2", "VAR:VOLATILES_GRAPH_OLD_1", "VAR:VOLATILES_GRAPH_CURRENT"], //EXTEND HOW MANY LABLES ARE DISPLAYED (MAYBE inside Settings)
    datasets: [
      {
        label: "Volatiles",
        backgroundColor: "#fff",
        borderColor: "#fff",
        data: [20, 800, 300, 400, 10, 50, 20],  //TODO HERE TO PUT THE LIVE VARIABLES
        fill: false,
        pointBorderWidth: 100,
        pointBorderColor: "transparent",
        pointRadius: 3,
        pointBackgroundColor: "transparent",
        pointHoverBackgroundColor: "rgba(63,82,227,1)",
      },
    ],
  },
  options: {
    responsive: true,
    maintainAspectRatio: false,
    layout: {
      padding: {
        left: -10,
        top: 10,
      },
    },
    legend: {
      display: false,
    },
    title: {
      display: false,
    },
    tooltips: {
      mode: "index",
      intersect: false,
    },
    hover: {
      mode: "nearest",
      intersect: true,
    },
    scales: {
      xAxes: [
        {
          gridLines: {
            drawBorder: false,
            display: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
      yAxes: [
        {
          gridLines: {
            display: false,
            drawBorder: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
    },
  },
};
var config_humidity = {
  type: "line",
  data: {
    labels: ["VAR:HUMIDITY_GRAPH_OLD_6", "VAR:HUMIDITY_GRAPH_OLD_5", "VAR:HUMIDITY_GRAPH_OLD_4", "VAR:HUMIDITY_GRAPH_OLD_3", "VAR:HUMIDITY_GRAPH_OLD_2", "VAR:HUMIDITY_GRAPH_OLD_1", "VAR:HUMIDITY_GRAPH_CURRENT"], //EXTEND HOW MANY LABLES ARE DISPLAYED (MAYBE inside Settings)
    datasets: [
      {
        label: "Humidity",
        backgroundColor: "#fff",
        borderColor: "#fff",
        data: [20, 40, 20, 200, 10, 540, 723],  //TODO HERE TO PUT THE LIVE VARIABLES
        fill: false,
        pointBorderWidth: 100,
        pointBorderColor: "transparent",
        pointRadius: 3,
        pointBackgroundColor: "transparent",
        pointHoverBackgroundColor: "rgba(63,82,227,1)",
      },
    ],
  },
  options: {
    responsive: true,
    maintainAspectRatio: false,
    layout: {
      padding: {
        left: -10,
        top: 10,
      },
    },
    legend: {
      display: false,
    },
    title: {
      display: false,
      text: "Chart.js Line Chart",
    },
    tooltips: {
      mode: "index",
      intersect: false,
    },
    hover: {
      mode: "nearest",
      intersect: true,
    },
    scales: {
      xAxes: [
        {
          gridLines: {
            drawBorder: false,
            display: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
      yAxes: [
        {
          gridLines: {
            display: false,
            drawBorder: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
    },
  },
};
var config_temperature = {
  type: "line",
  data: {
    labels: ["VAR:TEMPERATURE_GRAPH_OLD_6", "VAR:TEMPERATURE_GRAPH_OLD_5", "VAR:TEMPERATURE_GRAPH_OLD_4", "VAR:TEMPERATURE_GRAPH_OLD_3", "VAR:TEMPERATURE_GRAPH_OLD_2", "VAR:TEMPERATURE_GRAPH_OLD_1", "VAR:TEMPERATURE_GRAPH_CURRENT"], //EXTEND HOW MANY LABLES ARE DISPLAYED (MAYBE inside Settings)
    datasets: [
      {
        label: "Temperature",
        backgroundColor: "#fff",
        borderColor: "#fff",
        data: [20, 40, 20, 70, 10, 5, 23], //TODO HERE TO PUT THE LIVE VARIABLES
        fill: false,
        pointBorderWidth: 100,
        pointBorderColor: "transparent",
        pointRadius: 3,
        pointBackgroundColor: "transparent",
        pointHoverBackgroundColor: "rgba(63,82,227,1)",
      },
    ],
  },
  options: {
    responsive: true,
    maintainAspectRatio: false, 
    layout: {
      padding: {
        left: -10,
        top: 10,
      },
    },
    legend: {
      display: false,
    },
    title: {
      display: false,
      text: "Chart.js Line Chart",
    },
    tooltips: {
      mode: "index",
      intersect: false,
    },
    hover: {
      mode: "nearest",
      intersect: true,
    },
    scales: {
      xAxes: [
        {
          gridLines: {
            drawBorder: false,
            display: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
      yAxes: [
        {
          gridLines: {
            display: false,
            drawBorder: false,
          },
          ticks: {
            display: false,
          },
        },
      ],
    },
  },
};

var randomScalingFactor = function() {
  return (Math.random() > 0.5 ? 1.0 : 1.0) * Math.round(Math.random() * 100);
};

// draws a rectangle with a rounded top
Chart.helpers.drawRoundedTopRectangle = function(ctx, x, y, width, height, radius) {
  ctx.beginPath();
  ctx.moveTo(x + radius, y);
  // top right corner
  ctx.lineTo(x + width - radius, y);
  ctx.quadraticCurveTo(x + width, y, x + width, y + radius);
  // bottom right	corner
  ctx.lineTo(x + width, y + height);
  // bottom left corner
  ctx.lineTo(x, y + height);
  // top left	
  ctx.lineTo(x, y + radius);
  ctx.quadraticCurveTo(x, y, x + radius, y);
  ctx.closePath();
};

Chart.elements.RoundedTopRectangle = Chart.elements.Rectangle.extend({
  draw: function() {
    var ctx = this._chart.ctx;
    var vm = this._view;
    var left, right, top, bottom, signX, signY, borderSkipped;
    var borderWidth = vm.borderWidth;

    if (!vm.horizontal) {
      // bar
      left = vm.x - vm.width / 2;
      right = vm.x + vm.width / 2;
      top = vm.y;
      bottom = vm.base;
      signX = 1;
      signY = bottom > top? 1: -1;
      borderSkipped = vm.borderSkipped || 'bottom';
    } else {
      // horizontal bar
      left = vm.base;
      right = vm.x;
      top = vm.y - vm.height / 2;
      bottom = vm.y + vm.height / 2;
      signX = right > left? 1: -1;
      signY = 1;
      borderSkipped = vm.borderSkipped || 'left';
    }

    // Canvas doesn't allow us to stroke inside the width so we can
    // adjust the sizes to fit if we're setting a stroke on the line
    if (borderWidth) {
      // borderWidth shold be less than bar width and bar height.
      var barSize = Math.min(Math.abs(left - right), Math.abs(top - bottom));
      borderWidth = borderWidth > barSize? barSize: borderWidth;
      var halfStroke = borderWidth / 2;
      // Adjust borderWidth when bar top position is near vm.base(zero).
      var borderLeft = left + (borderSkipped !== 'left'? halfStroke * signX: 0);
      var borderRight = right + (borderSkipped !== 'right'? -halfStroke * signX: 0);
      var borderTop = top + (borderSkipped !== 'top'? halfStroke * signY: 0);
      var borderBottom = bottom + (borderSkipped !== 'bottom'? -halfStroke * signY: 0);
      // not become a vertical line?
      if (borderLeft !== borderRight) {
        top = borderTop;
        bottom = borderBottom;
      }
      // not become a horizontal line?
      if (borderTop !== borderBottom) {
        left = borderLeft;
        right = borderRight;
      }
    }

    // calculate the bar width and roundess
    var barWidth = Math.abs(left - right);
    var roundness = this._chart.config.options.barRoundness || 0.5;
    var radius = barWidth * roundness * 0.5;
    
    // keep track of the original top of the bar
    var prevTop = top;
    
    // move the top down so there is room to draw the rounded top
    top = prevTop + radius;
    var barRadius = top - prevTop;

    ctx.beginPath();
    ctx.fillStyle = vm.backgroundColor;
    ctx.strokeStyle = vm.borderColor;
    ctx.lineWidth = borderWidth;

    // draw the rounded top rectangle
    Chart.helpers.drawRoundedTopRectangle(ctx, left, (top - barRadius + 1), barWidth, bottom - prevTop, barRadius);

    ctx.fill();
    if (borderWidth) {
      ctx.stroke();
    }

    // restore the original top value so tooltips and scales still work
    top = prevTop;
  },
});

Chart.defaults.roundedBar = Chart.helpers.clone(Chart.defaults.bar);

Chart.controllers.roundedBar = Chart.controllers.bar.extend({
  dataElementType: Chart.elements.RoundedTopRectangle
});

var ctxBar = document.getElementById("bar").getContext("2d");
var myBar = new Chart(ctxBar, {
  type: 'bar',
  data: {
    labels: ["VAR:VOLATILE1", "VAR:VOLATILE2", "VAR:VOLATILE3", "VAR:VOLATILE4", "VAR:VOLATILE5", "VAR:VOLATILE6", "VAR:VOLATILE7"],
    datasets: [{
      //label: 'Students',
      backgroundColor: [chartColors.grey, chartColors.grey, chartColors.grey, chartColors.grey, chartColors.info, chartColors.blue, chartColors.grey],
      data: [5, 10, 30, 40, 35, 55, 15 ] //TODO PUT LIVE HATA HERE
    }]
  },
  options: {
    responsive: true,
    barRoundness: 1,
    title: {
      display: false,
      text: "Chart.js - Bar Chart with Rounded Tops (drawRoundedTopRectangle Method)"
    },
    legend: {
      display:false
    },
    scales: {
      yAxes: [{
        ticks: {
          beginAtZero: true,
          suggestedMax: 40 + 20,
          padding: 10,
        },
        gridLines: {
          drawBorder: false,
        }
      }],
      xAxes: [{
            gridLines: {
                display:false,
                drawBorder: false
            }
        }]
    }
  }
});
var radialBarsOptions = {
  series: [44, 80, 67],
  chart: {
    height: 350,
    type: "radialBar",
  },
  theme: {
    mode: "light",
    palette: "palette1",
    monochrome: {
      enabled: true,
      color: "#3245D1",
      shadeTo: "light",
      shadeIntensity: 0.65,
    },
  },
  plotOptions: {
    radialBar: {
      dataLabels: {
        name: {
          offsetY: -15,
          fontSize: "22px",
        },
        value: {
          fontSize: "2.5rem",
        },
        total: {
          show: true,
          label: "Volatiles",
          color: "#25A6F1",
          fontSize: "16px",
          formatter: function(w) {
            // By default this function returns the average of all series. The below is just an example to show the use of custom formatter function
            return "$4,124";
          },
        },
      },
    },
  },
  //labels: ["Apples", "Oranges", "Bananas", "Berries"],
};
var radialBars = new ApexCharts(document.querySelector("#radialBars"), radialBarsOptions);
radialBars.render();
let ctx_CO2                 = document.getElementById("canvas1").getContext("2d");
let ctx_volatiles           = document.getElementById("canvas2").getContext("2d");
let ctx_humidity            = document.getElementById("canvas3").getContext("2d");
let ctx_temperature         = document.getElementById("canvas4").getContext("2d");
var line_chart_CO2          = new Chart(ctx_CO2, config_CO2);
var line_chart_volatiles    = new Chart(ctx_volatiles, config_volatiles);
var line_chart_humidity     = new Chart(ctx_humidity, config_humidity);
var line_chart_temperature  = new Chart(ctx_temperature, config_temperature);
