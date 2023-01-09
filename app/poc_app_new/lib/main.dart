import 'dart:async';
import 'dart:math';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:line_chart/model/line-chart.model.dart';
import 'package:speedometer/speedometer.dart';
import 'package:rxdart/rxdart.dart';
import 'package:file_picker/file_picker.dart';
import 'package:line_chart/charts/line-chart.widget.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int counter = 0;
  double speed = 0;
  double rpm = 0;
  List<LineChartModel>speed_chart_data = [];
  List<LineChartModel>rpm_chart_data = [];

  int time = 0;
  int last_positive_speed_time = 0;

  String advice = "";

  PublishSubject<double> eventObservableSpeed = PublishSubject();
  PublishSubject<double> eventObservableRpm = PublishSubject();
  @override
  void initState() {
    super.initState();
  }

  void _handleSpeedChange(double newValue) {
    setState(() {
      eventObservableSpeed.add(newValue);
      speed = newValue;
      speed_chart_data.add(new LineChartModel(amount: speed, date : DateTime.now()));

      if (speed > 0) {
        last_positive_speed_time = time;
      }
    });

    _displayAdvice();
  }

  void _handleRpmChange(double newValue) {
    setState(() {
      eventObservableRpm.add(newValue);
      rpm = newValue;
      rpm_chart_data.add(new LineChartModel(amount: rpm, date : DateTime.now()));
    });

    _displayAdvice();
  }

  void _displayAdvice() {
    if (rpm > 1000 && time - last_positive_speed_time > 100) {
      setState(() {
        advice = "Donner des coups de gaz dans le vide ne sert à rien ;)";
      });
    }
    else if (rpm > 0 && time - last_positive_speed_time > 100) { // 10 secondes sans avancer avec moteur
      setState(() {
        advice = "Pensez à arrêter votre moteur lors des arrêts prolongés !";
      });
    }
    else if (rpm > 2250) {
      setState(() {
        advice = "Passez la vitesse supérieure !";
      });
    }
    else {
      setState(() {
        advice = "";
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    final ThemeData somTheme = ThemeData(
        primaryColor: Colors.blue,
        accentColor: Colors.black,
        backgroundColor: Colors.grey);

    final ThemeData rpmTheme = ThemeData(
        primaryColor: Colors.red,
        accentColor: Colors.black,
        backgroundColor: Colors.grey);
    return Scaffold(
        appBar: AppBar(
          title: Text("MyOBD"),
        ),
        body: Column(
          children: <Widget>[
            Row(
              children : <Widget>[
                Expanded(child:
                  Padding(
                    padding: EdgeInsets.all(40.0),
                    child: SpeedOMeter(
                        start: 0,
                        end: 130,
                        highlightStart: (10 / 130),
                        highlightEnd: (80 / 130),
                        themeData: somTheme,
                        eventObservable: this.eventObservableSpeed),
                  )
                )
              ,
              Expanded(child:
                Padding(
                  padding: EdgeInsets.all(40.0),
                  child: SpeedOMeter(
                      start: 0,
                      end: 6000,
                      highlightStart: (2500 / 6000.0),
                      highlightEnd: (6000 / 6000),
                      themeData: rpmTheme,
                      eventObservable: this.eventObservableRpm),
                )
              )
            ]),

            Text(advice, style: const TextStyle(fontWeight: FontWeight.bold, color: Colors.red),),

            Text("\nVitesse :"),

            LineChart(
              width: 300, // Width size of chart
              height: 180, // Height size of chart
              data: speed_chart_data, // The value to the chart
              linePaint: Paint()
                ..strokeWidth = 2
                ..style = PaintingStyle.stroke
                ..color = Colors.black, // Custom paint for the line
            ),

            Text("\nRégime moteur :"),

            LineChart(
              width: 300, // Width size of chart
              height: 180, // Height size of chart
              data: rpm_chart_data, // The value to the chart
              linePaint: Paint()
                ..strokeWidth = 2
                ..style = PaintingStyle.stroke
                ..color = Colors.black, // Custom paint for the line
            ),

            TextButton(
              style: ButtonStyle(
                foregroundColor: MaterialStateProperty.all<Color>(Colors.blue),
              ),
              onPressed: () async {
                FilePickerResult? result = await FilePicker.platform.pickFiles();

                if (result != null) {


                  File file = File(result.files.single.path!);
                  String fileString = await file.readAsString();
                  List<String> values = fileString.split("\n");
                  for (String v in values) {
                    String key = v.split(",")[0];
                    String value = v.split(",")[1];


                    if (int.parse(key) == 4) {
                      _handleSpeedChange(double.parse(value));
                    }
                    if (int.parse(key) == 3) {
                      _handleRpmChange(double.parse(value));
                    }

                    await Future.delayed(Duration(milliseconds: 100));
                    time++;

                  }
                }
              },
              child: Text('Importer des données'),
            ),


          ],
        ));
  }
}
