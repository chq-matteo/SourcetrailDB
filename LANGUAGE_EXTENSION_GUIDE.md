# Sourcetrail Language Extension Guide

## First Things First

Build on the shoulders of giants. There are many language tools out there that you can build upon, helping you to get things done much faster:

Look for a parser that generates an Abstract Syntax Tree (AST) for the language you want to index. The parser can be written in any language, but for parsing progamming languages these tools are mostly written in the language they are designed to parse. You will probably use the generated AST to visit certain nodes (e.g. all `function definition` nodes) and store all the useful information to the Sourcetrail database (e.g. a `function` with name `foo` is defined at location `<X:Y>`).

Having a parser is fine, but it is not enough. When you parse code that contains a function call, the parser may just create a `call expression` node for you. You may even be able to get the name of the called function from the AST node, but the parser certainly will not know where this function is actually defined. And if the indexed code defines two or more functions with the same name, you are in trouble.

That's why you also need to have a framework that can resolve this info for you. Just to name a few examples:

* for Java there is the [JavaParser](https://github.com/javaparser/javaparser) for building the AST and the [JavaSymbolSolver](https://github.com/javaparser/javasymbolsolver) (now integrated into JavaParser) for resolving these references.
* for C/C++ this is all done by the [Clang](https://clang.llvm.org/) compiler frontend.
* for Python the AST can be built by [parso](https://github.com/davidhalter/parso) while the references can be solved by [Jedi](https://github.com/davidhalter/jedi).

## Getting Started
Ok, let's assume that you found some awesome frameworks that help you build your Sourcetrail Language Package. So what's next?

* Start off by writing a small executable piece of code that reads in some input code and generates the AST.
* Implement an AST visitor that visits each node of the generated AST and prints that node's type. This functionality will come in handy for debugging your code later on.
* Make the SourcetrailDB interface accessible from your code base. You can either use [existing bindings](https://github.com/CoatiSoftware/SourcetrailDB#supported-language-bindings) for your desired language or open an issue at the [SourcetrailDB issues page](https://github.com/CoatiSoftware/SourcetrailDB/issues). If your language package has made it to this stage, the SourcetrailDB team will be happy to support your project!
* Extend the AST visitor to store information on definitions to the Sourcetrail database. This information should already be available from the AST, so you don't need any complex symbol solving yet.
