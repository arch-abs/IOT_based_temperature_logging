import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:async';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    // is not restarted.
    return MaterialApp(
      title: 'Temperature Monitoring',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final String url =
      "https://api.thingspeak.com/channels/746385/fields/1.json?results=2";
  List data;
  String temp = 'Loading...';

  String _now;
  Timer _everySecond;
  @override
  void initState() {
    super.initState();
    /*for (;;) {
      this.getJsonData();
    }*/

    this.getJsonData();
  }

  Future<String> getJsonData() async {
    var response = await http
        .get(Uri.encodeFull(url), headers: {"Accept": "application/json"});
    print("Going to print body :");
    print(response.body);

    setState(() {
      var toJsonData = json.decode(response.body);
      data = toJsonData['feeds'];
      int n = data.length;
      temp = data[n - 1]['field1'];
      print(temp);
    });

    return "Success";
  }

  /*void _incrementCounter() {
    setState(() {
      // This call to setState tells the Flutter framework that something has
      // changed in this State, which causes it to rerun the build method below
      // so that the display can reflect the updated values. If we changed
      // _counter without calling setState(), then the build method would not be
      // called again, and so nothing would appear to happen.
      _counter++;
    });
  }*/

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Teperature Monitor"),
      ),
      body: Center(
        child: Text(temp),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          getJsonData();
        },
        child: Icon(Icons.refresh),
      ),
    );
  }
}
