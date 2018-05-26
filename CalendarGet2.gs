/****************************************************

JSON format:

{
"success":"OK",
"eventsToday":
[
{"title":"Dentist Chi","time":" 11:00"},
{"title":"Forecast (27C | 24C)","time":" "}
etc
],
"eventsTomro":
[
{"title":"No events today","time":" "},
{"title":"Forecast (28C | 24C)","time":" "}
etc
]
}

or

{
"success":"NOK",
"error": errortype
}
*******************************************************/

function doGet(){
  //Logger.log(ContentService.createTextOutput(getResponse()));
  return ContentService.createTextOutput(getResponse());
}

function getResponse()
{
  var MILLIS_PER_DAY = 1000 * 60 * 60 * 24;
  var response = {};
  var eventsToday = []
  var eventsTomro = []
  
  var today = new Date();
  var tomorrow = new Date(today.getTime() + MILLIS_PER_DAY);
  cal = CalendarApp.getAllCalendars(); 
  
  if (cal) 
  {
    for (i in cal)
    {
      // check if we have Weather included in calendars.
      // weather is not an event so we need to discount it
      // to determine if no events today
      if (cal[i].getName().indexOf("Weather") >= 0)
        // yes
        weather = true;
      else 
        // no
        weather = false;
    }
    
    response.success = "OK";
    // do today first
    response.eventsToday = getEventsForDay( today, eventsToday );
    // now do tomorrow
    response.eventsTomro = getEventsForDay( tomorrow, eventsTomro );
  }
  else
  {
    response.success = "NOK";
    response.error = "Could not access calendars";
  }
  Logger.log(JSON.stringify(response));
  return JSON.stringify(response);
}

function getEventsForDay( day, eventarray )
{
  pushCount = 0;
  for (i in cal) // for each calendar
  {
    //Logger.log(cal[i].getName());
    // get all events for day
    var events = cal[i].getEventsForDay(day);
    
    if (events) // yes, some events
    {
      for (j in events) // for each event
      {       
        title = events[j].getTitle();
        // if this is forecast string re-format
        if (title.indexOf("Forecast") >= 0){
          var tstring = title.substring(title.indexOf("("), title.indexOf(")") + 1);
          // cant display degree sign so replace with 'C' (twice)
          tstring = tstring.replace("°","C");tstring = tstring.replace("°","C");
          tstring = "Forecast " + tstring;
          title = tstring;
        }
        // if it's an all day event then no time to display
        if (events[j].isAllDayEvent()) time = " ";
        // get the start time hour and minutes from event
        else time = events[j].getStartTime().toString().substring(15,21);
        
        pushCount = eventarray.push({"title": title, "time": time});
      }
    }
  }
  //Logger.log("pushCount = " + pushCount);
  if (weather)
  {
    if (pushCount == 1)  { // no events today (except forecast)
      // put 'No events' first then forecast
      var levent = eventarray.shift();
      title = "No events today";
      time = " ";
      eventarray.push({"title": title, "time": time});
      eventarray.push(levent);
    }
  }
  else // if no weather calendar
  {
    if (pushCount == 0 )
    {
      title = "No events today";
      time = " ";
      eventarray.push({"title": title, "time": time});
    }
  }
  return eventarray;
}
