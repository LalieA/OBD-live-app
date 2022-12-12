  import 'package:flutter/material.dart';

class Statistic extends StatefulWidget {
  Statistic(String this._name, double this._value) {
    currentState = _StatisticState();
  }

  String _name;
  double _value;

  String get name { return name;}
  double get value {return value;}

  set value (double v) {this.value = v; currentState.updateValue();}

  _StatisticState currentState;

  @override
  State<Statistic> createState() {
    currentState = _StatisticState();
    return currentState;
  }
}

class _StatisticState extends State<Statistic> {
  @override
  Widget build(BuildContext context) {
    return Text(widget.name + " : " + widget.value.toString() ); // TODO : nope
  }

  void updateValue() {
    setState(() {}); // TODO : check if useful
  }
}

