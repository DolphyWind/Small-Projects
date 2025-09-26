let my_name = "";
let connected = false;
const socket = new WebSocket('wss://codeguessing.gay/73/ws:443');
let buffer = "";

function write_msg(msg, newline) {
    process.stdout.clearLine(0);
    process.stdout.cursorTo(0);
    process.stdout.write(msg);
    if(newline) console.log();
}

socket.onmessage = (event) => {
    try {
        json = JSON.parse(event.data);
        reason = json.reason;
        if (reason === "connect") {
            name = json.name;
            if(!connected) {
                connected = true;
                write_msg(`My name is "${name}" ^_^`, true);
                write_msg(`> ${buffer}`);
                my_name = name;
            } else {
                shown_name = Math.random() < 0.001 ? "Herobrine" : name;
                write_msg(`${shown_name} has joined.`, true);
                write_msg(`> ${buffer}`);
            }
        } else if (reason === "message") {
            sender = json.name;
            if(sender !== my_name) {
                content = json.content;
                write_msg(`${sender}: ${content}`, true);
                write_msg(`> ${buffer}`);
            }
        } else if (reason === "disconnect") {
            name = json.name;
            write_msg(`${name} has quit.`, true);
            write_msg(`> ${buffer}`);
        } else {
            write_msg(`I don't know how to interpret this: ${event.data}`, true);
            write_msg(`> ${buffer}`);
        }
        
    } catch (e) {
        console.error(`Something bad happened, I better disconnect: ${e}`);
        socket.close();
    }
};

process.stdin.setRawMode(true);
process.stdin.resume();
process.stdin.setEncoding('utf8');

function isPrintable(char) {
    const code = char.charCodeAt(0);
    return (code >= 32 && code <= 126) ||
           (code >= 160 && code <= 55295) ||
           (code >= 57344 && code <= 1114111);
}

process.stdin.on('data', (key) => {
    const keyCode = key.charCodeAt(0);

    if (key === '\u0003') {
        socket.close();
        process.exit();
    } else if (keyCode === 13) {
        if(buffer === "" || !connected) return;
        buffer = buffer.replace('"', '\\"');
        socket.send(`{"content": "${buffer}"}`);
        write_msg(`${my_name}: ${buffer}`, true);
        buffer = "";
        write_msg("> ");
    } else if(keyCode === 127) {
        buffer = buffer.substr(0, buffer.length-1);
        write_msg(`> ${buffer}`);
    }
    if(!isPrintable(key)) return;

    buffer += key;
    write_msg(`> ${buffer}`);
});

process.stdin.on('end', () => {
    process.exit();
});

