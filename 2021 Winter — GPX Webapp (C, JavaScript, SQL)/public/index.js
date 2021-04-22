var waypoints = [];
var connected = false;

// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {
    // On page-load AJAX Example
    jQuery.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/endpoint1',   //The server endpoint we are connecting to
        data: {
            data1: "Value 1",
            data2:1234.56
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            jQuery('#blah').html("On page load, received string '"+data.somethingElse+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });

    updateFileLogData();
    updateGPXViewSelect();
    updateDBFileSelect();
    updateAddRouteSelect();

    hideDBOptions();
    hideDBSelect();
    hideDBResults();


    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible

    $('#fileSelect-button').click(function(e){
        e.preventDefault();
        getRoutesTracksForFile();
    });

    $('#componentSelect-button').click(function(e){
        $('#renameComponent-label').html("Rename " +$('#componentSelect').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
    });

    $('#renameComponent').submit(function(e){
        renameRouteTrack();
        e.preventDefault();
        if (connected) {
            updateDB();
        }
    });

    $('#showOtherData').click(function(e){
        getGPXData();
        e.preventDefault();
    });

    $('#createGPX').submit(function(e){
        e.preventDefault();
        let validNum = parseFloat($('#createGPX-version').val());
        if (isNaN(validNum)) {
            alert("Version must be a float. File creation failed.");
        } else {
            createGPXFile();
            if (connected) {
                updateDB();
            }
        }
    });

    $('#addroute-addRoute').click(function(e){
        e.preventDefault();
        addRoute();
        $('#addRoute-output').html("");
        if (connected) {
            updateDB();
        }
    });

    $('#addroute-addWaypoint').click(function(e){
        let tempWpt = {
            lat: parseFloat($('#addRoute-lat').val()),
            lon: parseFloat($('#addRoute-lon').val())
        }
        waypoints.push(tempWpt);
        console.log(waypoints);
        $('#addRoute-output').append("\nNew waypoint added: "+$('#addRoute-lat').val() + " " +$('#addRoute-lon').val());
        e.preventDefault();
    });

    $('#findPath').submit(function(e){
        findPathBetween();
        e.preventDefault();
    });

    $('#db-login').submit(async function(e){
        e.preventDefault();
        await loginDB();
    });

    $('#db-status').click(async function(e){
        e.preventDefault();
        await getDBStatus();
    });

    $('#db-storeFiles').click(async function(e){
        e.preventDefault();
        await updateDB();
        updateDBFileSelect();
    });

    $('#db-clearData').click(async function(e){
        e.preventDefault();
        await deleteDB();
    });

    $('#querySelect-button').click(async function(e){
        e.preventDefault();
        showDBResults();
        if ($('#db-queryList').val() == 'Display all routes (sort by name)') {
            await displayAllRoutesName();
        } else if ($('#db-queryList').val() == 'Display all routes (sort by length)') {
            await displayAllRoutesLength();
        } else if ($('#db-queryList').val() == 'Display all routes from file (sort by name)') {
            await displayFileRoutesName();
        } else if ($('#db-queryList').val() == 'Display all routes from file (sort by length)') {
            await displayFileRoutesLength();
        } else if ($('#db-queryList').val() == 'Display all points in route') {
            await displayRoutePoints();
        } else if ($('#db-queryList').val() == 'Display all points in file (sort by route name)') {
            await displayFilePointsName();
        } else if ($('#db-queryList').val() == 'Display all points in file (sort by route length)') {
            await displayFilePointsLength();
        } else if ($('#db-queryList').val() == 'Display n longest routes from file (sort by route name)') {
            await displayFileRoutesNameLongest();
        } else if ($('#db-queryList').val() == 'Display n longest routes from file (sort by route length)') {
            await displayFileRoutesLengthLongest();
        } else if ($('#db-queryList').val() == 'Display n shortest routes from file (sort by route name)') {
            await displayFileRoutesNameShortest();
        } else if ($('#db-queryList').val() == 'Display n shortest routes from file (sort by route length)') {
            await displayFileRoutesLengthShortest();
        } else {
            alert("No option selected");
        }
    });

    $('#db-fileListButton').click(async function(e){
        e.preventDefault();
        getRoutesForFile();
    });
    
});


function createGPXFile() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/createGPX',
        data: {
            filename: $('#createGPX-filename').val(),
            info: {
                version: parseFloat($('#createGPX-version').val()),
                creator: $('#createGPX-creator').val()
            }
        },
        success: function(data) {
            alert(data.message);
            console.log(data);
        },
        fail: function(error) {
            alert("Failed to create file on server");
            console.log(error);
        }
    });
    updateFileLogData();
    updateGPXViewSelect();
    updateAddRouteSelect();
    updateDBFileSelect();
};

function updateFileLogData() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileList',
        data: {},
        success: function(data) {
            console.log(data);
            let numFiles = data.filename.length;
            $('#fileLog').find("tr:not(:first)").remove();
            if (numFiles == 0) {
                let row = "<tr><th class='full-row' colspan = '6'>No files</th></tr>";
                $('#fileLog').append(row);
            } else{
                for (i = 0; i < numFiles; i++) {
                    let GPXData = JSON.parse(data.fileData[i]);
                    let row = "<tr><td><a href='" + data.filename[i] + "' download>" + data.filename[i] + "</td>";
                    row = row + "<td>" + GPXData.version + "</td>";
                    row = row + "<td>" + GPXData.creator + "</td>";
                    row = row + "<td>" + GPXData.numWaypoints + "</td>";
                    row = row + "<td>" + GPXData.numRoutes + "</td>";
                    row = row + "<td>" + GPXData.numTracks + "</td>";
                    row = row +"</tr>";
                    $('#fileLog').append(row);
                }
            }
        },
        fail: function(error) {
            alert("Failed to get file list for File Log panel");
            console.log(error);
        }
    });
}

function updateGPXViewSelect() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileList',
        data: {},
        success: function(data) {
            console.log(data);
            let numFiles = data.filename.length;
            $('#fileSelect-gpxView').empty();
            if (numFiles == 0) {
                let row = "<option>No files</option>";
                $('#fileSelect-gpxView').append(row);
            } else{
                for (i = 0; i < numFiles; i++) {
                    let row = "<option>" + data.filename[i] + "</option>";
                    $('#fileSelect-gpxView').append(row);
                }
            }
        },
        fail: function(error) {
            alert("Failed to get file list for GPX View panel");
            console.log(error);
        }
    });
}

function updateAddRouteSelect() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileList',
        data: {},
        success: function(data) {
            console.log(data);
            let numFiles = data.filename.length;
            $('#fileSelect-addRoute').empty();
            if (numFiles == 0) {
                let row = "<option>No files</option>";
                $('#fileSelect-addRoute').append(row);
            } else{
                for (i = 0; i < numFiles; i++) {
                    let row = "<option>" + data.filename[i] + "</option>";
                    $('#fileSelect-addRoute').append(row);
                }
            }
            getRoutesTracksForFile();
        },
        fail: function(error) {
            alert("Failed to get file list for Add Route panel");
            console.log(error);
        }
    });
}

function updateDBFileSelect() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileList',
        data: {},
        success: function(data) {
            console.log(data);
            let numFiles = data.filename.length;
            $('#db-fileList').empty();
            if (numFiles == 0) {
                let row = "<option>No files</option>";
                $('#db-fileList').append(row);
            } else{
                for (i = 0; i < numFiles; i++) {
                    let row = "<option>" + data.filename[i] + "</option>";
                    $('#db-fileList').append(row);
                }
            }
            getRoutesTracksForFile();
        },
        fail: function(error) {
            alert("Failed to get file list for Add Route panel");
            console.log(error);
        }
    });
}

function getRoutesForFile() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getRouteData',
        data: {
            filename: $('#db-fileList').val()
        },
        success: function(data) {
            console.log(data);
            let numRoutes = data.routes.length;
            $('#db-routeList').empty();
            if (numRoutes == 0) {
                row = "<option>No routes</option>";
                $('#db-routeList').append(row);
            } else {
                for (i = 0; i < numRoutes; i++) {
                    console.log("Adding Route to selector");
                    let routeData = data.routes[i];
                    let j = i + 1;
                    row = "<option>" + routeData.name + "</option>";
                    $('#db-routeList').append(row);
                }
            }
        },
        fail: function(error) {
            alert("Failed to get route and track data for GPX View panel");
            console.log(error);
        }
    });
}

function getRoutesTracksForFile() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getRouteTrackData',
        data: {
            filename: $('#fileSelect-gpxView').val()
        },
        success: function(data) {
            console.log(data);
            let numRoutes = data.routes.length;
            let numTracks = data.tracks.length;
            $('#routeTrackData').find("tr:not(:first)").remove();
            $('#componentSelect').empty();
            if ((numRoutes == 0) && (numTracks == 0)) {
                let row = "<tr><th class='full-row' colspan = '5'>No routes or tracks found for file: " + $('#fileSelect-gpxView').val() +"</th></tr>";
                $('#routeTrackData').append(row);
                row = "<option>No routes or tracks</option>";
                $('#componentSelect').append(row);
            } else {
                for (i = 0; i < numRoutes; i++) {
                    console.log("Adding Route");
                    let routeData = data.routes[i];
                    let j = i + 1;
                    let row = "<tr><td>Route " + j + "</td>";
                    row = row + "<td>" + routeData.name + "</td>";
                    row = row + "<td>" + routeData.numPoints + "</td>";
                    row = row + "<td>" + routeData.len + "</td>";
                    row = row + "<td>" + routeData.loop + "</td>";
                    row = row +"</tr>";
                    $('#routeTrackData').append(row);
                    row = "<option>Route " + j + "</option>";
                    $('#componentSelect').append(row);
                }
                for (i = 0; i < numTracks; i++) {
                    console.log("Adding Track");
                    let trackData = data.tracks[i];
                    let j = i + 1;
                    let row = "<tr><td>Track " + j + "</td>";
                    row = row + "<td>" + trackData.name + "</td>";
                    row = row + "<td>" + trackData.numPoints + "</td>";
                    row = row + "<td>" + trackData.len + "</td>";
                    row = row + "<td>" + trackData.loop + "</td>";
                    row = row +"</tr>";
                    $('#routeTrackData').append(row);
                    row = "<option>Track " + j + "</option>";
                    $('#componentSelect').append(row);
                }
            }
        },
        fail: function(error) {
            alert("Failed to get route and track data for GPX View panel");
            console.log(error);
        }
    });
}


function renameRouteTrack() {
    let subStr = $('#componentSelect').val().split(' ');
    console.log("New name sent to server: " +$('#renameComponentBox').val());
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/renameComponent',
        data: {
            filename: $('#fileSelect-gpxView').val(),
            type: subStr[0],
            num: subStr[1],
            name: $('#renameComponentBox').val()
        },
        success: function(data) {
            console.log(data);
            if (data.result == true) {
                alert("Changed component name successfully");
            } else {
                alert("Unable to change component name");
            }
            getRoutesTracksForFile();
        },
        fail: function(error) {
            alert("Failed to change component name");
            console.log(error);
        }
    });
}


function getGPXData() {
    let subStr = $('#componentSelect').val().split(' ');
    console.log("Getting GPX Data from server for: " +$('#componentSelect').val());
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getGPXDataFromFile',
        data: {
            filename: $('#fileSelect-gpxView').val(),
            type: subStr[0],
            num: subStr[1],
        },
        success: function(data) {
            console.log(data);
            let result = JSON.parse(data.d);
            if (result.length > 0) {
                let gpxData = "";
                for (let i = 0; i < result.length; i++) {
                    gpxData = gpxData + result[i].name + ": " + result[i].value + "\n";
                }
                alert(gpxData)
            } else {
                alert("No other data available");
            }
            //getRoutesTracksForFile();
        },
        fail: function(error) {
            alert("Failed to change component name");
            console.log(error);
        }
    });
}


function findPathBetween() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/findAllPaths',
        data: {
            accuracy: $('#findPath-acc').val(),
            start: {
                lat: $('#findPath-latStart').val(),
                lon: $('#findPath-lonStart').val()
            },
            end: {
                lat: $('#findPath-latEnd').val(),
                lon: $('#findPath-lonEnd').val()
            }
        },
        success: function(data) {
            console.log(data);
            let numRoutes = data.routes.length;
            let numTracks = data.tracks.length;
            $('#foundPaths').find("tr:not(:first)").remove();
            if ((numRoutes == 0) && (numTracks == 0)) {
                let row = "<tr><th class='full-row' colspan = '5'>No routes or tracks found for input coordinates</th></tr>";
                $('#foundPaths').append(row);
            } else {
                for (i = 0; i < numRoutes; i++) {
                    console.log("Adding Route");
                    let routeData = data.routes[i];
                    let j = i + 1;
                    let row = "<tr><td>Route " + j + "</td>";
                    row = row + "<td>" + routeData.name + "</td>";
                    row = row + "<td>" + routeData.numPoints + "</td>";
                    row = row + "<td>" + routeData.len + "</td>";
                    row = row + "<td>" + routeData.loop + "</td>";
                    row = row +"</tr>";
                    $('#foundPaths').append(row);
                }
                for (i = 0; i < numTracks; i++) {
                    console.log("Adding Track");
                    let trackData = data.tracks[i];
                    let j = i + 1;
                    let row = "<tr><td>Track " + j + "</td>";
                    row = row + "<td>" + trackData.name + "</td>";
                    row = row + "<td>" + trackData.numPoints + "</td>";
                    row = row + "<td>" + trackData.len + "</td>";
                    row = row + "<td>" + trackData.loop + "</td>";
                    row = row +"</tr>";
                    $('#foundPaths').append(row);
                }
            }
        },
        fail: function(error) {
            alert("Failed to get route and track data for Find Path panel");
            console.log(error);
        }
    });
}

function addRoute() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/addRouteToFile',
        data: {
            filename: $('#fileSelect-addRoute').val(),
            routename: $('#addRoute-name').val(),
            points: waypoints
        },
        success: function(data) {
            console.log(data);
            waypoints = [];
        },
        fail: function(error) {
            alert("Failed to create file on server");
            console.log(error);
        }
    });
}

async function loginDB() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/dbLogin',
        data: {
            username : $('#db-username').val(),
            password : $('#db-password').val(),
            dbName   : $('#db-dbname').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                alert("Successfully logged in to database");
                hideDBLogin();
                showDBOptions();
                showDBSelect();
                getDBStatus();
                connected = true;
            } else {
                alert("Failed to log in to database");   
            }
        },
        fail: function(error) {
            alert("Failed to log in to database");
            console.log(error);
        }
    });
}

function hideDBLogin() {
    document.getElementById("db-login").style.display = "none";
    
}

function showDBLogin() {
    document.getElementById("db-login").style.display = "block";
}

function hideDBOptions() {
    document.getElementById("db-optionButtons").style.display = "none";
    
}

function showDBOptions() {
    document.getElementById("db-optionButtons").style.display = "block";
}

function hideDBSelect() {
    document.getElementById("db-queryList").style.display = "none";
    document.getElementById("querySelect-button").style.display = "none";
    document.getElementById("db-queryFileSelect").style.display = "none";
    document.getElementById("db-queryRouteSelect").style.display = "none";
    document.getElementById("db-nRoutes").style.display = "none";
}

function showDBSelect() {
    document.getElementById("db-queryList").style.display = "block";
    document.getElementById("querySelect-button").style.display = "block";
    document.getElementById("db-queryFileSelect").style.display = "block";
    document.getElementById("db-queryRouteSelect").style.display = "block";
    document.getElementById("db-nRoutes").style.display = "block";
}

function hideDBResults() {
    document.getElementById("db-results").style.visibility = "hidden";
    
}

function showDBResults() {
    document.getElementById("db-results").style.visibility = "visible";
}


async function deleteDB() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/dbDelete',
        data: {},
        success: async function(data) {
            console.log(data);
            if (data.e) {
                alert("All data removed from database");
                await getDBStatus();

                $('#db-fileList').empty();
                $('#db-routeList').empty();
                let row = "<option>No files</option>";
                $('#db-fileList').append(row);
                row = "<option>No routes</option>";
                $('#db-routeList').append(row);
            } else {
                alert("Failed to remove all data from database");
                await getDBStatus();
            }
        },
        fail: async function(error) {
            alert("Failed to remove all data from database");
            await getDBStatus();
            console.log(error);
        }
    });
}


async function updateDB() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/dbDelete',
        data: {},
        success: async function(data) {
            console.log(data);
            await populateDB();
        },
        fail: function(error) {
            console.log(error);
        }
    });
}


async function populateDB() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/dbPopulate',
        data: {},
        success: async function(data) {
            console.log(data);
            if (data.e) {
                alert("Populated database successfully");
                await getDBStatus();
            } else {
                alert("Failed to populate database");
                await getDBStatus();
            }
        },
        fail: async function(error) {
            alert("Failed to populate database");
            await getDBStatus();
            console.log(error);
        }
    });
}

async function getDBStatus() {
        $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/dbStatus',
        data: {},
        success: function(data) {
            console.log(data);
            alert("Database contains: \n" + data.files + " files\n" + data.routes + " routes\n" + data.points + " points");
        },
        fail: function(error) {
            alert("Failed to get database status");
            console.log(error);
        }
    });
}


async function displayAllRoutesName() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getAllRoutesName',
        data: {},
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayAllRoutesLength() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getAllRoutesLength',
        data: {},
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayFileRoutesName() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileRoutesName',
        data: {
            filename : $('#db-fileList').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayFileRoutesLength() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileRoutesLength',
        data: {
            filename : $('#db-fileList').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayRoutePoints() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getRoutePoints',
        data: {
            filename : $('#db-fileList').val(),
            route    : $('#db-routeList').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numPoints = data.points.length;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Point ID</th><th>Point Index</th><th>Latitude</th><th>Longitude</th></tr>";
                $('#db-results').append(header);

                if (numPoints == 0) {
                    let row = "<tr><th class='full-row' colspan = '5'>No points found for route</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numPoints; i++) {
                        console.log("Adding Point to table");
                        let pointData = data.points[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (pointData.route_name == "") {
                            row = row + "<td> </td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + pointData.point_name + "</td>";
                        }
                        row = row + "<td>" + pointData.point_id + "</td>";
                        row = row + "<td>" + pointData.point_index + "</td>";
                        row = row + "<td>" + pointData.latitude + "</td>";
                        row = row + "<td>" + pointData.longitude + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get point data");
            }
        },
        fail: function(error) {
            alert("Failed to get point data");
            console.log(error);
        }
    });
}


async function displayFilePointsName() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getRoutePointsName',
        data: {
            filename : $('#db-fileList').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;
                let pointExist = false;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Route Name</th><th>Point Name</th><th>Point ID</th><th>Point Index</th><th>Latitude</th><th>Longitude</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '6'>No points found for route</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let routeName;
                        if (routeData.route_name == "") {
                            routeName = "Route " + nameless;
                            nameless++;
                        } else {
                            routeName = routeData.route_name;
                        }
                        
                        let numPoints = data.points[i].length;

                        if (numPoints > 0) {
                            pointExist = true;
                            for (let j = 0; j < numPoints; j++) {
                                let pointData = data.points[i][j];
                                let row = "<tr><td> " + routeName + "</td>";
                                if (pointData.route_name == "") {
                                    row = row + "<td> </td>";
                                } else {
                                    row = row + "<td>" + pointData.point_name + "</td>";
                                }
                                row = row + "<td>" + pointData.point_id + "</td>";
                                row = row + "<td>" + pointData.point_index + "</td>";
                                row = row + "<td>" + pointData.latitude + "</td>";
                                row = row + "<td>" + pointData.longitude + "</td>";
                                row = row +"</tr>";
                                $('#db-results').append(row);
                            }
                        } 
                    }
                    if (pointExist == false) {
                        let row = "<tr><th class='full-row' colspan = '6'>No points found for route</th></tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get point data");
            }
        },
        fail: function(error) {
            alert("Failed to get point data");
            console.log(error);
        }
    });
}



async function displayFilePointsLength() {
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getRoutePointsLength',
        data: {
            filename : $('#db-fileList').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;
                let pointExist = false;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Route Name</th><th>Point Name</th><th>Point ID</th><th>Point Index</th><th>Latitude</th><th>Longitude</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '6'>No points found for route</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let routeName;
                        if (routeData.route_name == "") {
                            routeName = "Route " + nameless;
                            nameless++;
                        } else {
                            routeName = routeData.route_name;
                        }
                        
                        let numPoints = data.points[i].length;

                        if (numPoints > 0) {
                            pointExist = true;
                            for (let j = 0; j < numPoints; j++) {
                                let pointData = data.points[i][j];
                                let row = "<tr><td> " + routeName + "</td>";
                                if (pointData.route_name == "") {
                                    row = row + "<td> </td>";
                                } else {
                                    row = row + "<td>" + pointData.point_name + "</td>";
                                }
                                row = row + "<td>" + pointData.point_id + "</td>";
                                row = row + "<td>" + pointData.point_index + "</td>";
                                row = row + "<td>" + pointData.latitude + "</td>";
                                row = row + "<td>" + pointData.longitude + "</td>";
                                row = row +"</tr>";
                                $('#db-results').append(row);
                            }
                        } 
                    }
                    if (pointExist == false) {
                        let row = "<tr><th class='full-row' colspan = '6'>No points found for route</th></tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get point data");
            }
        },
        fail: function(error) {
            alert("Failed to get point data");
            console.log(error);
        }
    });
}


async function displayFileRoutesNameLongest() {
    let num;
    if (isNaN($('#db-numRoutes').val())){
        num = 0;
    } else {
        num = $('#db-numRoutes').val();
    }
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileRoutesNameLongest',
        data: {
            filename : $('#db-fileList').val(),
            n        : $('#db-numRoutes').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayFileRoutesLengthLongest() {
    let num;
    if (isNaN($('#db-numRoutes').val())){
        num = 0;
    } else {
        num = $('#db-numRoutes').val();
    }
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileRoutesLengthLongest',
        data: {
            filename : $('#db-fileList').val(),
            n        : $('#db-numRoutes').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayFileRoutesNameShortest() {
    let num;
    if (isNaN($('#db-numRoutes').val())){
        num = 0;
    } else {
        num = $('#db-numRoutes').val();
    }
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileRoutesNameShortest',
        data: {
            filename : $('#db-fileList').val(),
            n        : $('#db-numRoutes').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}


async function displayFileRoutesLengthShortest() {
    let num;
    if (isNaN($('#db-numRoutes').val())){
        num = 0;
    } else {
        num = $('#db-numRoutes').val();
    }
     $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileRoutesLengthShortest',
        data: {
            filename : $('#db-fileList').val(),
            n        : $('#db-numRoutes').val()
        },
        success: function(data) {
            console.log(data);
            if (data.e) {
                let numRoutes = data.routes.length;
                let nameless = 1;

                $('#db-results').find("th").remove();
                $('#db-results').find("tr").remove();
                let header = "<tr><th>Name</th><th>Route ID</th><th>Length (m)</th></tr>";
                $('#db-results').append(header);

                if (numRoutes == 0) {
                    let row = "<tr><th class='full-row' colspan = '3'>No routes found for file</th></tr>";
                    $('#db-results').append(row);
                } else {
                    for (i = 0; i < numRoutes; i++) {
                        console.log("Adding Route to table");
                        let routeData = data.routes[i];
                        let j = i + 1;
                        let row = "<tr>";
                        if (routeData.route_name == "") {
                            row = row + "<td>Route " + nameless + "</td>";
                            nameless++;
                        } else {
                            row = row + "<td>" + routeData.route_name + "</td>";
                        }
                        row = row + "<td>" + routeData.route_id + "</td>";
                        row = row + "<td>" + routeData.route_len + "</td>";
                        row = row +"</tr>";
                        $('#db-results').append(row);
                    }
                }
                showDBResults();
            } else {
                alert("Failed to get route data");
            }
        },
        fail: function(error) {
            alert("Failed to get route data");
            console.log(error);
        }
    });
}