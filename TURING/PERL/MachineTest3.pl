#!/usr/bin/perl -w
# Jan.2008 initial v.0.1 Alpha/Pre-release in MKS Toolkit Perl on Windows XP
# March 2026 ported to Strawberry Perl (64 Bit) v.5.42.0.1 on Windows 10


################################################################################
#                       MODULE PACKAGE 
################################################################################
# Module Turing/MachineTest2.pl containing the package Turing::MachineTest2
package Turing::MachineTest2;

use strict;
use warnings;
#use diagnostics;
use Data::Dumper;
use Carp;

use Win32::Console::ANSI qw/ Cls Cursor /;
use FindBin qw($Bin);                     # Include current script dir in @INC 
use lib "$Bin";

use Turing::Machine;                      # Include Class Turin
select((select(STDOUT), $|=1)[0]);        # Autoflush STDOUT 


################################################################################
#                       Check TM postcondition
################################################################################
sub check {
   my ($TM, $exp) = @_;
   my $ones = grep (/1/, split(//, $TM->tape()));
   my $res = 'TM ' . $TM->id() . ($exp eq $ones ? ' OK' : ' ***ERROR***');
   $res .= '(' . $TM->get_hTrans() .'-'. $TM->get_hDelta() .'): ';
   $res .= " Expect:$exp, Got:" . $ones; 
   print $res, "\n";
}


################################################################################
#                       Busy Beaver TM's
################################################################################
# TM CTOR: 

{  # ===== Test BB candidates ==================================================
   my $pgm =                                                      # ones   steps
   'A0B1R A1B1L B0A1L B1H1R;'                                     # 4      6  
.  'A0B1R A1H1R B0B1L B1A1L;'                                     # 3      6  
.  'A0B1R A1B0L B0A1L B1H1R;'                                     # 3      6  

.  'A0B1L A1C1R B0C1L B1B1L C0D1L C1E0R D0A1R D1D1R E0H1L E1A0R;' # 4098 47176870
.  'A0B1L A1A1L B0C1R B1B1R C0A1L C1D1R D0A1L D1E1R E0H1R E1C0R;' # 4098 11798826
.  'A0B1L A1D0R B0C1R B1D1L C0A1R C1C1R D0H1L D1E1L E0A1L E1B0L;' # 4097 23554764
.  'A0B1L A1A1L B0C1R B1D1L C0A1R C1C1R D0H1L D1E0L E0C1R E1B1L;' # 4097 11798796
.  'A0B1L A1A1L B0C1R B1D0L C0A1R C1C1R D0H1L D1E1L E0E0R E1B1L;' # 4096 11804910
.  'A0B1L A1A1L B0C1R B1D0L C0A1R C1C1R D0H1L D1E1L E0C1R E1B1L;' # 4096 11804896
.  'A0B1L A1D1L B0C1R B1E0R C0A0L C1B0R D0E1L D1H1L E0C1R E1C1L'; # 1471  2358064

   $pgm =~ s/[\s]+//g;
   my @pgm = split(/;/, $pgm);
   print Data::Dumper::Dumper(\@pgm);

   my $TM = Turing::Machine->new("BB5_2",2,$pgm[3],30_000,'0',2,'',256,50_000_000);
   $TM->run(0x101);
   check($TM, '4098');
}


### END Turing::BeaverTest in BeaverTest.pl ####################################
