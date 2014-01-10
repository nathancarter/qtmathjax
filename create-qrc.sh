#!/bin/sh

# create list of all files to put in qrc
echo 'MathJax/MathJax.js' > qtmathjax.qrc.tmp
find MathJax/config/local -type f >> qtmathjax.qrc.tmp
find MathJax/config -type f -name *SVG* >> qtmathjax.qrc.tmp
for f in `ls MathJax/extensions/*.js`
do echo $f >> qtmathjax.qrc.tmp ; done
find MathJax/extensions/MathML -type f >> qtmathjax.qrc.tmp
find MathJax/extensions/TeX -type f >> qtmathjax.qrc.tmp
find MathJax/jax/element -type f >> qtmathjax.qrc.tmp
find MathJax/jax/input/MathML -type f >> qtmathjax.qrc.tmp
find MathJax/jax/input/TeX -type f >> qtmathjax.qrc.tmp
find MathJax/jax/output/SVG -type f >> qtmathjax.qrc.tmp
find MathJax/localization/en -type f >> qtmathjax.qrc.tmp
echo 'main.html' >> qtmathjax.qrc.tmp

echo '<RCC>' > qtmathjax.qrc
echo '  <qresource>' >> qtmathjax.qrc
grep -v DS_Store qtmathjax.qrc.tmp \
    | awk '{print "    <file>"$0"</file>"}' >> qtmathjax.qrc
echo '  </qresource>' >> qtmathjax.qrc
echo '</RCC>' >> qtmathjax.qrc

rm qtmathjax.qrc.tmp

