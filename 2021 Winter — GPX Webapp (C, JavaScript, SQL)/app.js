'use strict'

// C library API
var ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');


var sharedLib = ffi.Library('./parser/bin/libgpxparser', 
                {
                  'CreateGPXFromServer': ['string', [ 'string', 'string' ]],
                  'GetFileCharacters'  : ['string', [ 'string', 'string' ]],
                  'GetRouteData'       : ['string', [ 'string', 'string' ]],
                  'GetPointData'       : ['string', [ 'string', 'string', 'int' ]],
                  'GetTrackData'       : ['string', [ 'string', 'string' ]],
                  'ChangeRouteName'    : ['bool',   ['string', 'string', 'int', 'string' ]],
                  'ChangeTrackName'    : ['bool',   ['string', 'string', 'int', 'string' ]],
                  'GetGPXDataFromRoute': ['string', ['string', 'int', 'string' ]],
                  'GetGPXDataFromTrack': ['string', ['string', 'int', 'string' ]],
                  'ValidateFile'       : ['bool',   [ 'string', 'string' ]],
                  'GetRoutePathBetween': ['string', [ 'string', 'string', 'float', 'float', 'float', 'float', 'float' ]],
                  'GetTrackPathBetween': ['string', [ 'string', 'string', 'float', 'float', 'float', 'float', 'float' ]],
                  'AddRouteToFile'     : ['bool',   [ 'string', 'string', 'string']],
                  'AddWaypointToFile'  : ['bool',   [ 'string', 'string', 'string']]
                });

let dbConf = {
  host     : 'dursley.socs.uoguelph.ca',
  user     : 'usernameGoesHere',
  password : 'passwordGoesHere',
  database : 'databaseNameGoesHere'
};


app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }
    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 


app.get('/testFunction', function(req, res) {
  let retStr = "Waypoint: " + req.query.lat + " , " + req.query.lon;
  res.send(
    {
      waypointDesc: retStr
    }
  );
});

app.get('/createGPX', function(req, res) {
  req.query.info.version = parseFloat(req.query.info.version);
  let result = sharedLib.CreateGPXFromServer("./uploads/" + req.query.filename, JSON.stringify(req.query.info));
  //"Recieved JSON: " + req.query.filename + " " + req.query.info.version + " " + req.query.info.creator;
          //ffiLibraryFunctionCall(req.query.filename, req.query.info);
  res.send(
    {
      message: result
    });
});

app.get('/getFileList', function(req, res) {
  let filenames = fs.readdirSync('./uploads');
  let validFilenames = [];
  let data = [];
  let valid = false;
  let numFiles = filenames.length;
  for (let i = 0; i < numFiles; i ++) {
    valid = sharedLib.ValidateFile("./uploads/"+filenames[i], "gpx.xsd");
    if (valid == true) {
      validFilenames.push(filenames[i]);
      data.push(sharedLib.GetFileCharacters("./uploads/"+filenames[i], "gpx.xsd"));
    }
  }
  res.send({
    filename: validFilenames,
    fileData: data
  });
});

app.get('/getFileList2', function(req, res) {
  let filenames = fs.readdirSync('./uploads');
  res.send({
    filename: filenames,
  });
});

app.get('/getRouteTrackData', function(req, res) {
  let filename = "./uploads/" + req.query.filename;
  let routeData = JSON.parse(sharedLib.GetRouteData(filename, "gpx.xsd"));
  let trackData = JSON.parse(sharedLib.GetTrackData(filename, "gpx.xsd"));
  res.send({
    routes: routeData,
    tracks: trackData
  });
});

app.get('/getRouteData', function(req, res) {
  let filename = "./uploads/" + req.query.filename;
  let routeData = JSON.parse(sharedLib.GetRouteData(filename, "gpx.xsd"));
  res.send({
    routes: routeData
  });
});

app.get('/renameComponent', function(req, res) {
  let filename = "./uploads/" + req.query.filename;
  let type = req.query.type;
  let boolResult = false;
  if (type === 'Route') {
    boolResult = sharedLib.ChangeRouteName(filename, req.query.name, req.query.num, "gpx.xsd");
  } else if (type === 'Track') {
    boolResult = sharedLib.ChangeTrackName(filename, req.query.name, req.query.num, "gpx.xsd");
  } else {
    boolResult = false;
  }
  
  res.send({
    result: boolResult,
    name: req.query.name
  });
});

app.get('/getGPXDataFromFile', function(req, res) {
  let filename = "./uploads/" + req.query.filename;
  let type = req.query.type;
  let data = "";
  if (type === 'Route') {
    data = sharedLib.GetGPXDataFromRoute(filename, req.query.num, "gpx.xsd");
    //data = "It's a route";
  } else if (type === 'Track') {
    data = sharedLib.GetGPXDataFromTrack(filename, req.query.num, "gpx.xsd");
    //data = "It's a track";
  } else {
    data = "No Data";
  }
  res.send({
    result: req.query,
    file: filename,
    t: type,
    d: data
  });
});                

app.get('/findAllPaths', function(req, res) {
  let input = {
    accuracy: parseFloat(req.query.accuracy),
    start: {
      lat: parseFloat(req.query.start.lat),
      lon: parseFloat(req.query.start.lon)
    },
    end: {
      lat: parseFloat(req.query.end.lat),
      lon: parseFloat(req.query.end.lon)
    }
  };

  /*res.send({
    sent: input
  });*/

  let filenames = fs.readdirSync('./uploads');
  let validFilenames = [];
  let data = [];
  let valid = false;
  let numFiles = filenames.length;
  let tempRoute = [];
  let tempTrack = [];
  let routeData = [];
  let trackData = [];
  for (let i = 0; i < numFiles; i ++) {
    valid = sharedLib.ValidateFile("./uploads/"+filenames[i], "gpx.xsd");
    if (valid == true) {
      validFilenames.push("./uploads/"+filenames[i]);
    }
  }
  for (let i = 0; i < validFilenames.length; i++){
    tempRoute = JSON.parse(sharedLib.GetRoutePathBetween(validFilenames[i], "gpx.xsd", input.start.lat, input.start.lon, 
                input.end.lat, input.end.lon, input.accuracy));
    tempTrack = JSON.parse(sharedLib.GetTrackPathBetween(validFilenames[i], "gpx.xsd", input.start.lat, input.start.lon, 
                input.end.lat, input.end.lon, input.accuracy));
    for (let j = 0; j < tempRoute.length; j++) {
      routeData.push(tempRoute[j]);
    }
    for (let j = 0; j < tempTrack.length; j++) {
      trackData.push(tempTrack[j]);
    }
  }
  res.send({
    files: validFilenames,
    routes: routeData,
    tracks: trackData
  });
});



app.get('/addRouteToFile', function(req, res) {
  
  let route = {
    name: req.query.routename
  };
  let waypoints = req.query.points;
  let filename = "./uploads/"+req.query.filename;
  let result = false;
  let counter = 0;
  result = sharedLib.AddRouteToFile(filename, "gpx.xsd", JSON.stringify(route));
  if ((result == true) && (waypoints)) {
    for (let i = 0; i < waypoints.length; i++) {
      waypoints[i].lon = parseFloat(waypoints[i].lon);
      waypoints[i].lat = parseFloat(waypoints[i].lat);
      result = sharedLib.AddWaypointToFile(filename, "gpx.xsd", JSON.stringify(waypoints[i]));
      counter++;
    }
  }

  res.send({
    routeName: route,
    file: filename,
    points: waypoints,
    count: counter
  });

  /*
  let filename = "./uploads/" + req.query.filename;
  let type = req.query.type;
  let boolResult = false;
  if (type === 'Route') {
    boolResult = sharedLib.ChangeRouteName(filename, req.query.name, req.query.num, "gpx.xsd");
  } else if (type === 'Track') {
    boolResult = sharedLib.ChangeTrackName(filename, req.query.name, req.query.num, "gpx.xsd");
  } else {
    boolResult = false;
  }
  
  res.send({
    result: boolResult,
    name: req.query.name
  });
  */
});


app.get('/dbLogin', async function(req, res) {
  dbConf.user = req.query.username;
  dbConf.password = req.query.password;
  dbConf.database = req.query.dbName;
  // Open db

  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let numFiles, fields, numRoutes, fields1, numPoints, fields2, file, route, point;

  let query1 = "create table if not exists file (gpx_id int auto_increment, file_name varchar(60) not null, ver decimal(2,1) not null, creator varchar(256) not null, primary key (gpx_id))";
  let query2 = "create table if not exists route (route_id int auto_increment, route_name varchar(256), route_len decimal(15,7) not null, gpx_id int not null, primary key(route_id), foreign key(gpx_id) references file(gpx_id) on delete cascade)";
  let query3 = "create table if not exists point (point_id int auto_increment, point_index int not null, latitude decimal(11,7) not null, longitude decimal(11,7) not null, point_name varchar(60), route_id int not null, primary key(point_id), foreign key(route_id) references route(route_id) on delete cascade)";

  try {
    console.log(dbConf);
    connection = await mysql.createConnection(dbConf);
    await connection.execute(query1);
    await connection.execute(query2);
    await connection.execute(query3);
    
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      name : dbConf.user,
      word : dbConf.password,
      db   : dbConf.database,
      e    : result
    });
});

app.get('/dbDelete', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;

  try {
    connection = await mysql.createConnection(dbConf);
    await connection.execute("delete from point");
    await connection.execute("delete from route");
    await connection.execute("delete from file");
    
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e    : result
    });
}); 


app.get('/dbPopulate', async function(req, res) {

  let filenames = fs.readdirSync('./uploads');
  let validFilenames = [];
  let data;
  let valid = false;
  let numFiles = filenames.length;
  let query, gpx_id, route_id, field, route_data, point_data, str;
  let route_num = 0;
  let point_num = 0;
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;

  for (let i = 0; i < numFiles; i ++) {
    valid = sharedLib.ValidateFile("./uploads/"+filenames[i], "gpx.xsd");
    if (valid == true) {
      validFilenames.push(filenames[i]);
    }
  }

  try {
    connection = await mysql.createConnection(dbConf);

    numFiles = validFilenames.length;
    for (let i = 0; i < numFiles; i ++) {
      data = JSON.parse(sharedLib.GetFileCharacters("./uploads/"+validFilenames[i], "gpx.xsd"));

      query = "insert into file (file_name, ver, creator) values ('" + validFilenames[i] + "','" + data.version + "','" + data.creator + "')";
      await connection.execute(query);

      query = "select gpx_id from file where file_name = '" + validFilenames[i] + "'";
      [data, field] = await connection.execute(query);
      for (let row of data) {
        gpx_id = row.gpx_id;
      }
      console.log(data);

      route_data = JSON.parse(sharedLib.GetRouteData("./uploads/" + validFilenames[i], "gpx.xsd"));
      console.log(route_data);

      for (let route of route_data) {
        if (route.name == 'None'){
          query = "insert into route (route_name, route_len, gpx_id) values (NULL,'" + route.len + "','" + gpx_id + "')";
        } else {
          query = "insert into route (route_name, route_len, gpx_id) values ('" + route.name + "','" + route.len + "','" + gpx_id + "')";
        }
        await connection.execute(query);
        console.log(query);

        query = "select route_id from route where route_name = '" + route.name + "' AND gpx_id = '" + gpx_id + "'";
        [data, field] = await connection.execute(query);
        for (let row of data) {
          route_id = row.route_id;
        }
        console.log(data);

        str = sharedLib.GetPointData("./uploads/" + validFilenames[i], "gpx.xsd", route_num);
        console.log(str);
        point_data = JSON.parse(str);
        console.log(point_data);

        for (let point of point_data) {
          if (point.name === ""){
            query = "insert into point (point_index, latitude, longitude, point_name, route_id) values ('" + point.index + "','" + point.lat + "','" + point.lon + "',"  +   "NULL"   +  ",'" + route_id + "')";
          } else {
            query = "insert into point (point_index, latitude, longitude, point_name, route_id) values ('" + point.index + "','" + point.lat + "','" + point.lon + "','" + point.name + "','" + route_id + "')";
          }
          await connection.execute(query);
          console.log(query);

          point_num++;
        }
        route_num++;
        point_num = 0;
      }
      route_num = 0;
    }
  } catch(e) {
    console.log("Query error: "+e);
    console.log(route_data);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e    : result
    });
}); 


app.get('/dbStatus', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let numFiles, fields, numRoutes, fields1, numPoints, fields2, file, route, point;

  let query1 = 'select count (distinct gpx_id) FROM file';
  let query2 = 'select count (distinct route_id) FROM route';
  let query3 = 'select count (distinct point_id) FROM point';


  try {
    connection = await mysql.createConnection(dbConf);
    [numFiles, fields] = await connection.execute(query1);
    [numRoutes, fields1] = await connection.execute(query2);
    [numPoints, fields2] = await connection.execute(query3);

    for (let row of numFiles) {
      file = row["count (distinct gpx_id)"];
    }
    for (let row of numRoutes) {
      route = row["count (distinct route_id)"];
    }
    for (let row of numPoints) {
      point = row["count (distinct point_id)"];
    }

    console.log(numFiles);
    console.log(numRoutes);
    console.log(numPoints);
    console.log(file + "   " + route + "   " + point);
    
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e    : result,
      files  : file,
      routes : route,
      points : point
    });
}); 


app.get('/getAllRoutesName', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, file, route, point;

  let query1 = 'select * from route order by route_name';


  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    console.log(routeData);
    
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
}); 


app.get('/getAllRoutesLength', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, file, route, point;

  let query1 = 'select * from route order by route_len';


  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    console.log(routeData);
    
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});


app.get('/getAllRoutesLength', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, file, route, point;

  let query1 = 'select * from route order by route_len';

  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});


app.get('/getFileRoutesName', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, gpx_id, route, point;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from route where gpx_id = '" + gpx_id + "' order by route_name";

    [routeData, fields] = await connection.execute(query2);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});


app.get('/getFileRoutesLength', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, gpx_id, route, point;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from route where gpx_id = '" + gpx_id + "' order by route_len";

    [routeData, fields] = await connection.execute(query2);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});


app.get('/getRoutePoints', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let pointData, fields, gpx_id, route, point, route_id;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [pointData, fields] = await connection.execute(query1);

    for (let row of pointData) {
      gpx_id = row.gpx_id;
    }
    
    let query2 = "select route_id from route where gpx_id = '" + gpx_id + "' and route_name = '" + req.query.route + "'";
    [pointData, fields] = await connection.execute(query2);
    for (let row of pointData) {
      route_id = row.route_id;
    }
    console.log(route_id);

    let query3 = "select * from point where route_id = '" + route_id + "' order by point_index";

    [pointData, fields] = await connection.execute(query3);

    console.log(pointData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      points : pointData
    });
});


app.get('/getRoutePointsName', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, pointData, fields, gpx_id, route, point;
  let pointArr = [];

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from route where gpx_id = '" + gpx_id + "' order by route_name";
    [routeData, fields] = await connection.execute(query2);
    console.log(routeData);

    for (let row of routeData) {
      let route_id = row.route_id;
      let query3 = "select * from point where route_id = '" + route_id + "' order by point_index";
      [pointData, fields] = await connection.execute(query3);
      console.log(pointData);
      pointArr.push(pointData);
    }

    console.log(pointArr);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      points : pointArr,
      routes : routeData
    });
});


app.get('/getRoutePointsLength', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, pointData, fields, gpx_id, route, point;
  let pointArr = [];

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from route where gpx_id = '" + gpx_id + "' order by route_len";
    [routeData, fields] = await connection.execute(query2);
    console.log(routeData);

    for (let row of routeData) {
      let route_id = row.route_id;
      let query3 = "select * from point where route_id = '" + route_id + "' order by point_index";
      [pointData, fields] = await connection.execute(query3);
      console.log(pointData);
      pointArr.push(pointData);
    }

    console.log(pointArr);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      points : pointArr,
      routes : routeData
    });
});

app.get('/getFileRoutesNameLongest', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, gpx_id, route, point;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from (select * from route where gpx_id = '" + gpx_id + "' order by route_len desc limit " + req.query.n + ") temp order by route_name";

    [routeData, fields] = await connection.execute(query2);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});

app.get('/getFileRoutesLengthLongest', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, gpx_id, route, point;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from (select * from route where gpx_id = '" + gpx_id + "' order by route_len desc limit " + req.query.n + ") temp order by route_len";

    [routeData, fields] = await connection.execute(query2);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});

app.get('/getFileRoutesNameShortest', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, gpx_id, route, point;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from (select * from route where gpx_id = '" + gpx_id + "' order by route_len limit " + req.query.n + ") temp order by route_name";

    [routeData, fields] = await connection.execute(query2);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});

app.get('/getFileRoutesLengthShortest', async function(req, res) {
  const mysql = require('mysql2/promise');
  let connection;
  let result = true;
  let routeData, fields, gpx_id, route, point;

  let query1 = "select gpx_id from file where file_name = '" + req.query.filename + "'";
  
  try {
    connection = await mysql.createConnection(dbConf);
    [routeData, fields] = await connection.execute(query1);

    for (let row of routeData) {
      gpx_id = row.gpx_id;
    }
    let query2 = "select * from (select * from route where gpx_id = '" + gpx_id + "' order by route_len limit " + req.query.n + ") temp order by route_len";

    [routeData, fields] = await connection.execute(query2);

    console.log(routeData);
  } catch(e) {
    console.log("Query error: "+e);
    result = false;
  } finally {
    if (connection && connection.end) {
      connection.end();
    }
  }

  res.send({
      e      : result,
      routes : routeData
    });
});




//Sample endpoint
app.get('/endpoint1', function(req , res){
  let retStr = req.query.data1 + " " + req.query.data2;
  res.send(
    {
      somethingElse: retStr
    });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);