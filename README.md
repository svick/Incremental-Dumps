[Incremental dumps][1] are an improved format for dumps of content from WikiMedia wikis.

## Compiling

### Linux

You will need cmake 2.8 and gcc 4.8 (gcc 4.7 could work too, I haven't tested that).
To compile this project, run:

    cmake .
    make

### Windows

Open the solution in Visual Studio 2012 (Visual Studio Express 2012 could work too, I haven't tested that) and build it.

## Running the application

Compiling produces a command-line application `idumps` (or `idumps.exe` on Windows).

Running it wihtout parameters produces a short usage message, explaining possible actions and the meaning of their parameters.

### For dump readers

The following actions are useful for normal users of dumps, i.e. those who want to download and process them, not to create their own dumps.
In the future, there might be a special version of `idumps` that contains only these actions.

#### Reading a dump

The `r` (or `read`) action is for reading a dump and converting it to XML.
It takes two parameters: path to the dump file and path to the generated XML file.
If the XML file already exists, it will be overwritten.

Example:

    idumps r dump.id dump.xml

#### Applying a diff dump

The `a` (or `apply`) action is for applying diff dump to existing normal dump to update it.
It takes two parameters: path to the dump file and path to the diff dump file.
If the diff cannot be applied to the dump (because it's for different wiki, or for a dump with different timestamp), an error is printed.
Applying the diff also updates the timestamp of the dump.

Example:

    idumps a dump.id diff.dd

### For dump creators

The following actions are for dump creators, usually those who want to create a dump of their wiki.

#### Updating a dump from wiki

The `u` (or `update`) action is for updating or creating a dump based on communication with a wiki.
The first five parameters are:

* Name of the wiki (e.g. `enwiki`). This has to match with the name that's already in the dump, if it exists.

* New timestamp for the updated dump. This has to be different than the current timestamp in the dump, if it exists.

 Timestamps are used in diff dumps, to make sure only the right diff can be applied to the dump.

* Path to the PHP interpreter. This can be simply `php`, if `php` is in `PATH`.

* Path to the dumpBackup maintenance script, possibly with optional parameters (e.g. `--report`).

 The script will be called with additional parameters `--full --stub`, which are added automatically.

* Path to the fetchText maintenance script.

The remaining parameters specify what dumps to update/create, each group of two or three parameters represents separate dump.

The parameters in each group are:

* Dump specification.

* Path to the dump file.

* Path to the created diff dump (if diff was included in the specification).

 If the file already exists, it will be overwritten.

The specification is a 2- to 4-letter string that describes what kind of dump to create:

* 1. letter: `p` for pages dump or `s` for stub dump
* 2. letter: `h` for history dump or `c` for current dump
* 3. optional letter: `a` for articles dump (without talk and User namespaces)
* 4. optional letter: `d` to also create diff dump

Example:

    idumps u enwiki 20130823 php "/var/www/maintenance/dumpBackup.php --report=10000" /var/www/maintenance/fetchText.php pca pages-articles.id shd stub-history.id stub-history.dd

This sets the name of the wiki to `enwiki`, timestamp to `20130823`. The PHP interpreter is in `PATH`, MediaWiki is installed in `/var/www` and dumpBackup will report each 10000 revisions.

The updated or created dumps are a pages-current-articles dump pages-articles.id and a stub-history dump stub-history.id that also has a diff dump stub-history.dd.

### Creating a dump from XML

The `c` (or `create`) action is for creating incremental dump based on pages-history XML dump.

The parameters are similar as in the `update` action:

* Name of the wiki.
* Timestamp of the created dump.
* Path to the source XML dump. If the path is `-`, the source XML is read from the standard input. This can be useful for reading a compressed XML file without an intermediary file.

The remaining parameters specify what dumps to creates, just as in `update`.

There is also an optional parameter `--report` which has to be followed by a number *n* specifying that progress should be reported every *n* revisions.
If *n* is 0, progress reporting is turned off. If this parameter is specified, it has to come right before the “name of the wiki” parameter.

Example:

    idumps c enwiki 20130823 enwiki-20130823-pages-meta-history.xml sc sc.id

[1]: http://www.mediawiki.org/wiki/User:Svick/Incremental_dumps
