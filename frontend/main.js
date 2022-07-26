const dataPath = 'https://leomath42-weather-iot.herokuapp.com/api/v1/weather';
let tempMean, humidityMean;
let d;

const datebox = document.querySelector('.date-box');
datebox.addEventListener('keypress', setQuery);

function setQuery (event) {
    if (event.keyCode == 13) {
        datebox.blur();
        getFile(dataPath);
    }
}

function setValues (d) {
    const city = document.querySelector('.location .city');
    city.innerText = 'Rio de Janeiro, RJ';

    const date = document.querySelector('.location .date');
    date.innerText = dateBuilder(d);

    const temp = document.querySelector('.current .temp');
    temp.innerHTML = `${Math.round(tempMean)}<span>°c<span>`;

    const humidity = document.querySelector('.current .humidity');
    humidity.innerText = `${Math.round(humidityMean)}%`;

    const tempGraph = document.querySelector('.tempGraph');
    tempGraph.style.display = 'flex';

    const humidityGraph = document.querySelector('.humidityGraph');
    humidityGraph.style.display = 'flex';
}

function dateBuilder (d) {
    const days = ['Domingo', 'Segunda-Feira', 'Terça-Feira', 'Quarta-Feira', 'Quinta-Feira', 'Sexta-Feira', 'Sábado'];
    const months = ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'];

    dt = isDate(d)

    const day = days[dt.getDay()];
    const date = dt.getDate();
    const month = months[dt.getMonth()];
    const year = dt.getFullYear();

    return `${day}, ${date} de ${month} de ${year}`;
}

function isDate(d) {
    year = d.substr(0,4);
    month = d.substr(5,2);
    day = d.substr(8,2);
    return new Date(year, month, day);
}

const getFile = async file => {
    const response = await fetch(file);
    let rawFile = [];
    rawFile = await response.text();

    const data = intoArray(rawFile);

    const filter = filterData(data, datebox.value);

    d = filter;

    getMeans();
    getGraphs();
    setValues(datebox.value);
}

function intoArray (file) {
    const array = file.replaceAll('}', '}.}').replaceAll('[','').replaceAll('.}]\n','').split('.}, ');
    for (let i=0;i<array.length;i++){
        array[i] = JSON.parse(array[i]);
    }
    return array;
}

function getMeans () {

    let tempSum = 0, humiditySum = 0;
    for (let i=0;i<d.length;i++) {
        tempSum += d[i]['temperature'];
        humiditySum += d[i]['humidity'];
    }
    tempMean = tempSum / d.length;
    humidityMean = humiditySum / d.length;
}

function getGraphs(){
    // Load the Visualization API and the corechart package.
    google.charts.load('current', {'packages':['corechart']}).then(drawChart);
}

// Callback that creates and populates a data table,
// instantiates the pie chart, passes in the data and
// draws it.
function drawChart () {
    const tContainer = document.querySelector('#tGraph');
    const hContainer = document.querySelector('#hGraph');
    const tRow = [], hRow = [];

    const time = fixTime();

    for (let i=0;i<d.length;i++) {
        tRow[i] = [time[i], d[i]['temperature']];
        hRow[i] = [time[i], d[i]['humidity']];
    }
    // Create the data table.
    let tData = new google.visualization.DataTable();
    tData.addColumn('string', 'time');
    tData.addColumn('number', 'temperature');
    tData.addRows(tRow);

    let hData = new google.visualization.DataTable();
    hData.addColumn('string', 'time');
    hData.addColumn('number', 'humidity');
    hData.addRows(hRow);

    // Set chart options
    let tOptions = {'width':400,
                    'height':300,
                    'vAxes': {
                        0: {'title': 'Temperatura'},
                        1: {'title': 'Horário'}
                    }};

    let hOptions = {'width':400,
                    'height':300,
                    'vAxes': {
                        0: {'title': 'Umidade'},
                        1: {'title': 'Horário'}
                    }};

    // Instantiate and draw our chart, passing in some options.
    let tChart = new google.visualization.LineChart(tContainer);
    tChart.draw(tData, tOptions);

    let hChart = new google.visualization.LineChart(hContainer);
    hChart.draw(hData, hOptions);
}

function fixTime () {
    const fixed = []
    for (let i=0;i<d.length;i++) {
        fixed[i] = d[i]['time'].slice(11, 19);
    }
    return fixed;
}

function filterData (data, date) {

    const filter = [];
    for (let i=0;i<data.length;i++) {
        if (data[i]['time'].includes(date)) {
            filter[i] = data[i];
        }
    }
    return filter;
}