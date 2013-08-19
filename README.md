[Incremental dumps][1] are an improved format for dumps of content from WikiMedia wikis.

== Compiling ==

=== Linux ===

You will need cmake 2.8 and gcc 4.8 (gcc 4.7 could work too, I haven't tested that).
To compile this project, run:

    cmake .
    make

=== Windows ===

Open the solution in Visual Studio 2012 (Visual Studio Express 2012 could work too, I haven't tested that) and build it.

== Running the application ==

Compiling produces a command-line application `idumps`.

It can be used to convert pages-history XML dump to one or more types of incremental dumps
and also to convert an incremental dump back to XML.
Running it wihtout parameters produces a short usage message, explaining the meaning of parameters.

When creating a dump with a file name that already exists, the program tried to use the existing file.
This can cause problems, so if an error happens, delete the dump file and try running the command again

=== Examples ===

    idumps c tenwiki-20130622-pages-meta-history.xml sh sh.id pca pca.id

Creates stub-history dump `sh.id` and pages-articles dump `pca.id` from the XML dump of tenwiki.

    idumps r sh.id sh.xml

Creates `sh.xml`, XML version of the dump `sh.id`.


[1]: http://www.mediawiki.org/wiki/User:Svick/Incremental_dumps
