#! /usr/bin/perl

use strict;
use warnings;

use Time::Local 'timelocal_nocheck';

my (@lis, $fil1, $fil2, @splt1, @splt2);

my ($s1, $m1, $h1, $y1, $mon1, $d1, $tim1);
my ($s2, $m2, $h2, $y2, $mon2, $d2, $tim2);
my ($i, $j);
sub main {
  my $count=0;
  open(RTL, "rtlist2009");
  my @lis = <RTL>;

  for($i=0; $i < $#lis; $i++) {
    @splt1 = split('_', $lis[$i]);
    $splt1[5] =~ s/\.dat\n//;
    $h1 = substr($splt1[5], 0, 2); 
    $m1 = substr($splt1[5], 2, 2); 
    $s1 = substr($splt1[5], 4, 2); 
    $y1 = substr($splt1[4], 0, 4);
    $mon1 = substr($splt1[4], 4, 2);
    $d1 = substr($splt1[4], 6, 2);
    $tim1 = timelocal_nocheck($s1,$m1,$h1,$d1,$mon1,$y1);
    for($j=$i+1; $j < $#lis; $j++) {
      @splt2 = split('_', $lis[$j]);
      $splt2[5] =~ s/\.dat\n//;
      $h2 = substr($splt2[5], 0, 2); 
      $m2 = substr($splt2[5], 2, 2); 
      $s2 = substr($splt2[5], 4, 2); 
      $y2 = substr($splt2[4], 0, 4);
      $mon2 = substr($splt2[4], 4, 2);
      $d2 = substr($splt2[4], 6, 2);
      $tim2 = timelocal_nocheck($s2,$m2,$h2,$d2,$mon2,$y2);
#      print " ";
#      print $tim2 + 0 -$tim1;
#      print " ";

      if(abs($tim2-$tim1) > (12*3600-1800) && abs($tim2-$tim1) < (12*3600+1800)) {
	print "$lis[$i]";
	print "$lis[$j]";
	$count++;
      }
    }
  }
  print "TOTAL IS $count\n";


}

main;
0;
