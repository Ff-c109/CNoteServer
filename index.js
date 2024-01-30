/*const spawn = require("child_process").spawn
const c_listener = spawn("./c_listen")

c_listener.stdout.on("data", (data) => {
	console.log(data.toString())
})

c_listener.stdin.write("echo\nHello world!\n")*/
var tokens = new Array();
const spawn = require("child_process").spawn;
var http = require('http');
var querystring = require('querystring');
var fs = require('fs');

http.createServer(function (req, res) {
	if(req.url == "/") {
		res.writeHead(302, {'Location': '/login'});
		res.end();
	}
	if(req.url == "/login") {
  var body = "";
  req.on('data', function (chunk) {
    body += chunk;
  });
  req.on('end', function () {
    // 解析参数
    body = querystring.parse(body);
    // 设置响应头部信息及编码
    res.setHeader('Content-Type', 'text/html; charset=utf8');

    if(body.name && body.passwd) { // 输出提交的数据
	const c_listener = spawn("./cmain");

	c_listener.stdout.on("data", (data) => {
		console.log(data.toString());
		if(data.toString() == "true" || data.toString() == "true\n")
		{
			do {
				var token = "";
				for(var i = 0; i < 24; i++) {
					var tokenC = Math.random();
					tokenC = Math.floor(tokenC * 36);
					if(tokenC <= 9)
						token += tokenC.toString();
					else
						token += String.fromCharCode(65+(tokenC - 10));
				}
				var exist = false;
				tokens.forEach(t => {
					if(t == token)
						exist=true;
				});
				if(!exist) break;
			}
			while(true);

			tokens.push({"token" : token, "name" : body.name});
			console.log(token);
			res.setHeader('Set-Cookie', 'token=' + token + '; path=/;');
			res.end("<script>window.location = \"/login\"; </script>");
		}
		else
		{res.write("Wrong!");res.end();}
	});

	    c_listener.stdin.write("auth\n" + body.name + " " + body.passwd + "\n");

    } else {  // 输出表单
	    fs.readFile("login.html", (err, data) => {
		    if(err)
			    console.log(err);
		    else
			    res.end(data.toString());
	    });
    }

  });
	}
	else if(req.url == "/list") {
		fs.readFile("list.html", (err, data) => {
			if(err)
				console.log(err);
			else {
				res.write(data);
				res.end();
			}
		});
	}
	else if(req.url.toString().split('#')[0] == "/notetaking") {
		var body = "";
		req.on('data', function(chunk) {
			body += chunk;
		});
		req.on('end', function() {
			body = querystring.parse(body);
			if(body.noteName && body.text && body.token) {
				var token = body.token;
				var avaliable = false;
				var uname;
				tokens.forEach((tokenInfo) => {
					if(tokenInfo.token == token) {
						avaliable = true;
						uname = tokenInfo.name;
					}
				});
				if(!avaliable){
					res.end("token invalid");
				}
				else {
					const cmain = spawn("./cmain");
					cmain.on("exit", () => {
				fs.readFile("notetaking.html", (err, data) => {
					if(err)
						console.log(err);
					else {
						res.write(data);
						res.end();
					}
				});
					});
					cmain.stdin.write("writenote\n");
					cmain.stdin.write(uname);
					cmain.stdin.write(" ");
					cmain.stdin.write(body.noteName.split('#')[1]);
					cmain.stdin.write(" ");
					cmain.stdin.write(body.text);
					cmain.stdin.write("\n");
				}
			}
			else if(body.deleteL == "delete") {
				var token = body.token;
				var avaliable = false;
				var uname;
				tokens.forEach((tokenInfo) => {
					if(tokenInfo.token == token) {
						avaliable = true;
						uname = tokenInfo.name;
					}
				});
				if(!avaliable){
					res.end("token invalid");
				}
				else {
					const cmain = spawn("./cmain");
					cmain.stdout.on("data", d => {
						console.log(d);
					});
					cmain.on("exit", () => {
						res.end("<a href=\"/list\">Succeed</a>");
					});
					cmain.stdin.write("deletenote\n");
					cmain.stdin.write(uname);
					cmain.stdin.write(" ");
					cmain.stdin.write(body.noteName.split('#')[1]);
					cmain.stdin.write("\n");
				}

			}
			else {
				fs.readFile("notetaking.html", (err, data) => {
					if(err)
						console.log(err);
					else {
						res.write(data);
						res.end();
					}
				});
			}
		});


	}
	else if(req.url.toString().split('/')[1] == "api") {
		var splitedURL = req.url.toString().split('/');
		var pageName = splitedURL[2];
		var token = splitedURL[3];
		var uname;
		var avaliable = false;
		tokens.forEach((tokenInfo) => {
			if(tokenInfo.token == token) {
				avaliable = true;
				uname = tokenInfo.name;
			}
		});
		if (!avaliable) {
			console.log(token);
			res.write("This is an API Page.");
			res.end();
		}
		else {
			if(pageName == "notelist") {
				const cmain = spawn("./cmain");
				cmain.stdout.on("data", (data) => {
					res.write(data.toString());
				});
				cmain.on("exit", () => res.end());
				cmain.stdin.write("listnote\n");
				cmain.stdin.write(uname);
				cmain.stdin.write("\n");
			}
			else if(pageName == "readNote") {
				const cmain = spawn("./cmain");
				cmain.stdout.on("data", (data) => {
					res.write(data.toString());
				});
				cmain.on("exit", () => res.end());
				cmain.stdin.write("readnote\n");
				cmain.stdin.write(uname);
				cmain.stdin.write(" ");
				cmain.stdin.write(splitedURL[4]);
				cmain.stdin.write("\n");
			}
		}
	}
	else {
		fs.readFile("." + req.url.toString(), (err, d) => {
			if(err)
				res.end(err.toString());
			else
				res.end(d);
		});
	}
}).listen(3000);
