all: Calculator.class Main.class

Calculator.class: Calculator.java
	javac -d . Calculator.java

Main.class: Main.java Calculator.class
	javac Main.java
