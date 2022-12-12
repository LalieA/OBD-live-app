
class Data {

  static void init() {
    _stats["test1"] = 0.1;
    _stats["test2"] = 0.2;
    _stats["test3"] = 0.3;
    _stats["test5"] = 8;
  }

  static void statSet(String key, double value) {
    _stats[key] = value;
  }

  static Map<String, double> get stat {
    return _stats;
  }

  static Map<String, double> _stats = new Map<String, double>();

  
}