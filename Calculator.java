package Calculate;
import java.util.Stack;
import java.util.Queue;
import java.util.LinkedList

public class Calculator {
  private class Token {
    enum SymbolType {
      NUMBER,
      OPERATOR
    };
    enum SymbolType TokenType;
  }

  public LinkedList<Double> tokenize(String expression) {
    Queue<Token> tokens = new LinkedList<>;
    return tokens;
  }
  public double calc(String expression) {
    System.out.println(expression);
    return 1.234;
  }
}
