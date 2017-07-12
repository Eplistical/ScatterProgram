#!/bin/bash

sed -i s/"int "/"INT_T "/g *.*pp
sed -i s/"<int>"/"<INT_T>"/g *.*pp

sed -i s/"size_t "/"UINT_T "/g *.*pp
sed -i s/"<size_t>"/"<UINT_T>"/g *.*pp

sed -i s/"double "/"DOUBLE_T "/g *.*pp
sed -i s/"<double>"/"<DOUBLE_T>"/g *.*pp

sed -i s/"bool "/"BOOL_T "/g *.*pp
sed -i s/"<bool>"/"<BOOL_T>"/g *.*pp

sed -i s/"char "/"CHAR_T "/g *.*pp
sed -i s/"<char>"/"<CHAR_T>"/g *.*pp

sed -i s/"std::string "/"string "/g *.*pp
sed -i s/"<std::string>"/"<string>"/g *.*pp

sed -i s/"string "/"STRING_T "/g *.*pp
sed -i s/"<string>"/"<STRING_T>"/g *.*pp
