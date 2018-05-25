/*

JSON format:

{
"success":"OK",
"eventsToday":[
{"title":"Dentist Chi","time":" 11:00"},
{"title":"Forecast (27° | 24°)","time":" "}
etc
],
"eventsTomro":[
{"title":"No events today","time":" "},
{"title":"Forecast (28° | 24°)","time":" "}
etc
]
}

or

{
"success":"NOK",
"error": errortype
}

*/

function doGet(){
  Logger.log(ContentService.createTextOutput(getResponse()));
  return ContentService.createTextOutput(getResponse());
}
var cal;
var today;
var tomorrow;


function getResponse()
{
  var MILLIS_PER_DAY = 1000 * 60 * 60 * 24;
  var response = {};
  response.success = "OK";
  var eventsToday = []
  var eventsTomro = []
  var title = "";
  var time = "" ;
  
  today = new Date();
  tomorrow = new Date(today.getTime() + MILLIS_PER_DAY);
  cal = CalendarApp.getAllCalendars(); 
  if (cal) 
  {
    // do today first
    response.eventsToday = eventsToday;
    var k = 0;
    for (i in cal)
    {
      var events = cal[i].getEventsForDay(today);
     
      if (events) // some events
      {
        for (j in events)
        {
          title = events[j].getTitle();
          Logger.log(j + ">" + title);
          // if this is forecast string
          if (title.indexOf("Forecast") >= 0){
            var tstring = title.substring(title.indexOf("("), 1+title.indexOf(")"));
            tstring = tstring.replace("°","C");tstring = tstring.replace("°","C");
            tstring = "Forecast "+tstring;
            Logger.log(tstring);
            title = tstring;
          }
          if (events[j].isAllDayEvent())
          {
            time = " ";
          }
          else
          {
            // get the start time hour and minutes from event
            starttime = events[j].getStartTime().toString().substring(15,21);
            time = starttime;
          }
          k = response.eventsToday.push({"title": title, "time": time});
        }
      }
    }
    
    if (k == 1)  { // no events today except forecast
      var levent = response.eventsToday.shift();
      title = "No events today";
      time = " ";
      response.eventsToday.push({"title": title, "time": time});
      response.eventsToday.push(levent);
    }
    // now do tomorrow
    response.eventsTomro = eventsTomro;
    var k = 0;
    for (i in cal)
    {
      var events = cal[i].getEventsForDay(tomorrow);
      if (events) // some events
      {
        for (j in events)
        {
          title = events[j].getTitle();
          Logger.log(j + ">" + title);
          // if this is forecast string
          if (title.indexOf("Forecast") >= 0){
            var tstring = title.substring(title.indexOf("("), 1+title.indexOf(")"));
            tstring = tstring.replace("°","C");tstring = tstring.replace("°","C");
            tstring = "Forecast "+tstring;
            Logger.log(tstring);
            title = tstring;
          }
          if (events[j].isAllDayEvent())
          {
            time = " ";
          }
          else
          {
            // get the start time hour and minutes from event
            starttime = events[j].getStartTime().toString().substring(15,21);
            time = starttime;
          }
          k = response.eventsTomro.push({"title": title, "time": time});
        }
      }
    }
    
    if (k == 1)  { // no events today except forecast
      var pevent = response.eventsTomro.shift();
      title = "No events today";
      time = " ";
      response.eventsTomro.push({"title": title, "time": time});
      response.eventsTomro.push(pevent);
    }
  }
  
  else
  {
    response.success = "NOK";
    response.error = "Could not access calendars";
  }
  Logger.log(JSON.stringify(response));
  return JSON.stringify(response);
}
