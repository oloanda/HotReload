@ECHO OFF

ECHO --------------------------------------------------
ECHO --------------------------------------------------


SET WildCard=*.h *.cpp *.inl

ECHO TODOS FOUND:
findstr -s -n -i -l "TODO" %WildCard%




ECHO --------------------------------------------------
ECHO --------------------------------------------------