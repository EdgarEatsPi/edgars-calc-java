import java.util.Stack;
import java.util.Queue;

import Calculate.Calculator;

public class Main {
  public static void main(String args[]) {
    if (args.length == 0) {
      System.out.print("Usage: java Main <expression>\n");
      return;
    }

    Calculator calca = new Calculator();
    double results = calca.calc(args[0]);

    System.out.println(results);
  }
}
