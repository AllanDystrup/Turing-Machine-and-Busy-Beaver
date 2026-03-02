#!/usr/bin/perl -w
use strict;
use warnings;
use Data::Dumper;


# Assumptions:
# * the DATA fragments are sorted on fstart
# * basepair distance for one fragment is: abs(fstart, fstop)

my ($line, @raw, @sorted);

while ( chomp( $line = <DATA> )) {
   $line =~ s/\s+/-/g;
   push @raw, $line;
}
@sorted = sort @raw;
print Data::Dumper::Dumper(\@sorted);



__DATA__
1      105     1       14.5
1       105     1       14.5
1       105     1       14.5
1       813     797     4
1       813     797     22
1       813     797     4
1       813     797     22
1       800     816     23
1       802     818     24
1       804     820     32
1       804     820     44
1       813     797     4
1       813     797     22