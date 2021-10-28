
const ws = new WebSocket("wss://73.220.219.183:3000");
//const elem = document.getElementById('main');
ws.onopen = function (event) {
  // send binary echo header
  console.log("connected");
  ws.binaryType = "arraybuffer";
  let ab = new ArrayBuffer(17);
  let d = new DataView(ab);
  d.setUint8(2, 0x02); //raida id
  //d.setUint8(3, 0x00) //shard id
  d.setUint8(12, 0xAB);// echo
  d.setUint8(13, 0xAB);// echo
    ws.send(ab);
    console.log("echo sent");
    //ws.send("hello");
};
let r
ws.addEventListener('message', function (event) {
    r = event.data;
    let d = new DataView(r);

    console.log('Message from server ', d.getUint8(13).toString(16));
    console.log(d.getUint8(14).toString(16));
	//main.innerHTML = "Success";
    ws.close();
});

ws.addEventListener('error', function (event){

  console.log(event);
  //main.innerHTML = "Failed<br>"+event.message;
  ws.close();
});
//})})
