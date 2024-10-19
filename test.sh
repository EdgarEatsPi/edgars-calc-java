#! bin/bash

expression="1+1*2-3/4" # Should equal 2.25
java Main
java Main $expression
