# **Code style**
1. Indentation
    * **Tabs**: this way people can actually use whatever indentation they prefer, and it saves a few bytes here and there when it comes to file size.
1. Newlines
    * Use **Unix-style newlines (LF)**, per favore.
1. Naming conventions.
    * C
        * We work in the context of a library whose purpose it is to be integrated by perhaps other libraries or programs, thus we will prefix our...:
            1. ... functions and structs with `ls_`;
            2. ... macros and other definitions with `LS_`;
            3. ... significant others with a big sum of cash.
1. Order
    * C
        * Include files
            1. Copyleftrightupdown
            1. *Three* newlines.
            1. Header guard.