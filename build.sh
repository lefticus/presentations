#/usr/bin/sh

pandoc -t revealjs -s ThinkingPortable.md -o ThinkingPortable.html -A revealjs-header.html --css reveal.js/css/theme/lefticus.css
wkhtmltopdf --footer-right [page] -O Landscape -s Letter "file:///`pwd`/ThinkingPortable.html?print-pdf" ThinkingPortable.pdf

pandoc -t revealjs -s CurrentStateOfStaticAnalysis.md -o CurrentStateOfStaticAnalysis.html -A revealjs-header.html --css reveal.js/css/theme/lefticus.css
wkhtmltopdf --footer-right [page] -O Landscape -s Letter "file:///`pwd`/CurrentStateOfStaticAnalysis.html?print-pdf" CurrentStateOfStaticAnalysis.pdf

