import 'package:flutter/material.dart';
import 'package:poc_app/Statistic.dart';
import 'Data.dart';

class StatisticsList extends StatefulWidget {
  const StatisticsList({Key? key}) : super(key: key);

  @override
  State<StatisticsList> createState() => _StatisticsListState();
}

class _StatisticsListState extends State<StatisticsList> {
  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      padding: const EdgeInsets.all(16.0),
      itemCount: Data.stat.length * 2,
      itemBuilder: (context, i) {
        final index = i ~/ 2;

        double value = Data.stat.values.elementAt(index);
        String key = Data.stat.keys.elementAt(index);

        if (i.isOdd) return const Divider();

        return Statistic(key, value);
        
      },
    );
  }
}
