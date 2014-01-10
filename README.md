
# Overview

*QtMathJax* is a `.pri` file that lets you easily add beautiful
math typesetting into any [Qt] application.

[MathJax] is the de facto standard for mathematical typesetting on
the web, using a JavaScript-based typesetting engine.
*QtMathJax* imports just the necessary parts of that project into
your Qt project, runs them in a hidden `QWebView`, and
provides you a one-function API for turning a string of TeX code
into a string of SVG code that can be used to render it anywhere,
[in your Qt app][qtsvg] or outside it.

# Obtaining

Just clone this git repo, or use it as a [submodule] in your
project.  In fact, it uses [MathJax] as a [submodule] itself.

# Usage

To use *QtMathJax*, follow these steps.

1. In your `.pro` file, import *QtMathJax* by adding the following
   line.

```
include(/path/to/qtmathjax.pri)
```

2. In the source file where you need to use it, include the one
   class you need, `TeXEngine`.

```
#include "texengine.h"
```

3. Create an instance of the class and call the `TeX2SVG()` method
   in it.

```
TeXEngine engine;
// ...
QString svgCode = engine.TeX2SVG( "ax^2+bx+c=0" );
```

4. Render that SVG in any of [the ways Qt provides][qtsvg].

```
myQSvgWidget->load( svgCode.toUtf8() );
```

# Example

The repository comes with a sample app in the [example] subfolder.
If using Qt 5, you should be able to compile and run that app
without any changes needed.  It looks like this:

![Screenshot](./screenshot.png)

[Qt]: http://qt-project.org
[MathJax]: http://mathjax.org
[qtsvg]: http://qt-project.org/doc/qt-5.0/qtsvg/svgrendering.html
[submodule]: http://schacon.github.io/git/user-manual.html#submodules
[example]: ./example/

