% Vlad Dogaru, 2009. Based on thesis.cs.pub.ro, by Mircea Bardac,
% 2008-2009
% ddvlad@rosedu.org
% practic.cs.pub.ro.cls - class for practical exam
% Really just a shameless wrapper over the exam class by Philip
% Hirschhorn

% Copyright notice: you must keep the original author(s)/copyright
% information in the files

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{so.cs.pub.ro}[2010/01/21]

\LoadClassWithOptions{beamer}

% uncomment the lines bellow for Romanian Language support
% Use UTF8 file encoding when writing .tex files in Romanian
% Romanian Language support
\usepackage{ucs}
\usepackage[utf8x]{inputenc}
\PrerenderUnicode{aâîțșĂÎÂȚȘ}
\usepackage[english,romanian]{babel}
\usepackage{color}	  % if we need highlight
\usepackage{alltt}	  % if we need highlight
\usepackage{hyperref}	  % use \url{http://$URL} or \href{http://$URL}{Name}
\usepackage{underscore}	  % underscores need not be escaped

\mode<presentation>
{ \usetheme{so} }

% Support for including graphics
\usepackage{graphicx}
\DeclareGraphicsExtensions{.pdf,.png,.jpg}

% So that we don't have to declare it a bazillion times
\institute[SO]{Sisteme de Operare}

% Slide-urile cu mai multe părți sunt marcate cu textul (cont.)
\setbeamertemplate{frametitle continuation}[from second]

% Fix verbatim environment: smaller text size
\let\oldverbatim=\verbatim
\renewenvironment{verbatim}{
  \begin{small}
  \begin{oldverbatim}
  %\vspace*{1em}
}%
{%
  %\vspace*{1em}
  \end{oldverbatim}
  \end{small}
}

% command for specifying TODOs
\newcommand{\todo}[1]{
  \textbf{TODO:} #1 \par
}

% command for formatting file or directory names
\newcommand{\file}[1]{\texttt{#1}}

% command for formatting inline commands
\newcommand{\cmd}[1]{\textbf{\texttt{#1}}}
% command for formatting inline function names
\newcommand{\func}[1]{\textbf{\texttt{#1}}}

% Show contents at every section beginning. Ripped off from manual.
\AtBeginSection[] % Do nothing for \section*
{
  \begin{frame}<beamer>
    \frametitle{Outline}
  \tableofcontents[currentsection]
    \end{frame}
}
