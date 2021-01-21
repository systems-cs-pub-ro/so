% Heavily based on
% http://tug.ctan.org/tex-archive/info/latexcheat/latexcheat/
% Modified by Vlad Dogaru, February 2010
% License: LaTeX Project Public License

% Just scroll down to "\begin{document}" to start adding content.
% Also read the README file for custom commands.

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{refcard.cs.pub.ro}[2010/12/20]

\LoadClass[10pt,a4paper]{article}

% Here be dragons <<<

%\documentclass[10pt,a4paper,landscape]{article}
\usepackage[utf8x]{inputenc}
\PrerenderUnicode{ăĂâÂîÎșȘțȚ}
\usepackage[romanian,english]{babel}
\usepackage{multicol}
\usepackage{calc}
\usepackage{ifthen}
\usepackage[landscape]{geometry}
\usepackage{underscore}

% This sets page margins to .5 inch if using letter paper, and to 1cm
% if using A4 paper. (This probably isn't strictly necessary.)
% If using another size paper, use default 1cm margins.
\ifthenelse{\lengthtest { \paperwidth = 11in}}
	{ \geometry{top=.5in,left=.5in,right=.5in,bottom=.5in} }
	{\ifthenelse{ \lengthtest{ \paperwidth = 297mm}}
		{\geometry{top=1cm,left=1cm,right=1cm,bottom=1cm} }
		{\geometry{top=1cm,left=1cm,right=1cm,bottom=1cm} }
	}

% Turn off header and footer
\pagestyle{empty}

% Redefine section commands to use less space
\makeatletter
\renewcommand{\section}{\@startsection{section}{1}{0mm}%
                                {-1ex plus -.5ex minus -.2ex}%
                                {0.5ex plus .2ex}%x
                                {\normalfont\large\bfseries}}
\renewcommand{\subsection}{\@startsection{subsection}{2}{0mm}%
                                {-1explus -.5ex minus -.2ex}%
                                {0.5ex plus .2ex}%
                                {\normalfont\normalsize\bfseries}}
\renewcommand{\subsubsection}{\@startsection{subsubsection}{3}{0mm}%
                                {-1ex plus -.5ex minus -.2ex}%
                                {1ex plus .2ex}%
                                {\normalfont\small\bfseries}}
\makeatother

% Don't print section numbers
\setcounter{secnumdepth}{0}

\setlength{\parindent}{0pt}
\setlength{\parskip}{0.2cm}

% >>>

% SO Custom commands <<<

\newcommand{\func}[1]{\textbf{\texttt{#1}}} % Function name

% Tabular environment for function parameters
% TODO: The table should have customizable column with, but I don't know
% how to do that at the moment.
\newenvironment{params}
  {\\\begin{tabular}{@{}p{0.15\columnwidth}p{0.8\columnwidth}@{}}}
  {\end{tabular}}

% Parameter name; use in a `params' environment.
\newcommand{\param}[2]{\texttt{#1} & #2 \\}

% State what a function returns; use in a `params' environment.
\newcommand{\returns}[1]{\textit{întoarce} & #1 \\}

% >>>


