#!/usr/bin/perl -w

use strict;

my $path = "";
my %ignore = ( "old_main.cpp", 1 );

print "set (FRAMEWORK_SRC\n";

while (<>) {
    chomp;
    if ( /src\/(.*):/ )  {
        my $temp = $1;
        $path = ((length $temp) and $temp . "/" or "");
    } elsif ( (/^.+\.h$/ or /^.+\.cc$/) and not $ignore{$_} ) {
        print "    \${SRC_DIR}/$path$_\n";
    }
}

print ")\n"

