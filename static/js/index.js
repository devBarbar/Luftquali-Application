var ws = new WebSocket("ws://localhost:5000/ws");

ws.onopen = function() {
                  
    // Web Socket is connected, send data using send()
    ws.send("Message to send");
    alert("Message is sent...");
 };
  
 ws.onmessage = function (evt) { 
    var received_msg = evt.data;
    alert(received_msg);
 };
  
 ws.onclose = function() { 
    
    // websocket is closed.
    alert("Connection is closed...."); 
 };




