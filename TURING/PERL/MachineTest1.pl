#!/usr/bin/perl -w
# Jan.2008 initial v.0.1 Alpha/Pre-release in MKS Toolkit Perl on Windows XP
# March 2026 ported to Strawberry Perl (64 Bit) v.5.42.0.1 on Windows 10


################################################################################
#                       MODULE PACKAGE
################################################################################
# Module Turing/MachineTest1.pl containing the package Turing::MachineTest1
package Turing::MachineTest1;

use strict;
use warnings;
#use diagnostics;
use Data::Dumper;
use Carp;

use Win32::Console::ANSI;
use FindBin qw($Bin);            # Include current script dir in @INC 
use lib "$Bin";			 # "$Bin/..";

use Turing::Machine;             # Include Class Turing


################################################################################
#                       Check TM postcondition
################################################################################
sub check {
   my ($TM, $exp) = @_;
   my $match = $TM->tape();   # Default: match whole tape; Number: match 1's
   if ($exp =~ /[0-9]+/) { $match = grep (/1/, split(//, $TM->tape())); }
   my $res = 'TM ' . $TM->id() . ($exp eq $match ? ' OK' : ' ***ERROR***'); 
   $res .= '(' . $TM->get_hTrans() .'-'. $TM->get_hDelta() .'): ';
   $res .= " Expect:$exp, Got:" . $match; 
   print $res, "\n";
}


################################################################################
#                       UnitTest : Odd-Even TM's
################################################################################

{  # ===== Test case OE ========================================================
	# OddEven predicate solver

   my $pgm  = 
   #  +----------------------------------------------+
   #  |     INPUT        |           OUTPUT          |
   #  | State   Symbol   |   State   Symbol   Move   |
   #  +------------------+---------------------------+
      '   1       X            2       b       2     '
   .  '   1       b____________0       E      (2)    '
   .  '   2       X            1       b       2     '
   .  '   2       b            0       O      (2)    ';
   #  +----------------------------------------------+
   $pgm =~ s/[\s()_]+//g;  # 1X2b21b0E22X1b22b0O2


   {  # ===== Test case OE1: InpOdd
      my $TM = Turing::Machine->new("OE1-InpOdd",2,$pgm,6,'b',2,'X',80,50);
      $TM->run();
      check($TM, 'bbbbOb');
   }

   {  # ===== Test case OE2: InpEven
      my $TM = Turing::Machine->new("OE2-InpEven",2,$pgm,7,'b',2,'XX',80,50);
      $TM->run();
      check($TM, 'bbbbbEb');
   }

   {  # ===== Test case OE3: InpEmpty
      my $TM = Turing::Machine->new("OE3-InpEmpty",2,$pgm,4,'b',2,'',80,50);
      $TM->run();
      check($TM, 'bbEb');
   }

   {  # ===== Test case OE4: TapeLimit
      my $TM = Turing::Machine->new("OE4-TapeLimit",2,$pgm,7,'b',2,'XX',80,50);
      $TM->run();
      check($TM, 'bbbbbEb');
   }

   {  # ===== Test case OE5: TapeOverflow
      my $TM = Turing::Machine->new("OE5-TapeOverflow",2,$pgm,3,'b',2,'XX',80,50);
      eval { $TM->run(); }; warn $@ if $@;
      check($TM, 'bbb'); 
   }
}


################################################################################
#                       UnitTest : Unary Arithmetic TM's
################################################################################


{  # ===== Test case UA1 =======================================================
	# Unary Arithmetic 1 : Unary Addition
	# For TAPE[> 11110111], EXPECT:HALT[> 1111111]
	my $pgm =
      "1 2 2|10---|11---" .  		# Move R to first '1'
      "2 ---|20310|21212" .		# Move R to '0', erase w. '1'
      "3 4 1|30---|31312" .		# Move to R-most 1
      "4 ---|40---|415 0" .		# Erase it 
      "5 6 1|50---|51---" .		# Move L to first ' '
      "6 0 0|60---|61611";
   $pgm =~ s/\|//g;
   
   {  # 4 + 3 = 7
      my $TM= Turing::Machine->new("UA1.1",6,$pgm,256,' ',3,' 11110111',256,1000);
      $TM->run();
      check($TM, '7');
   }
   
   {  # 5 + 4 = 9
      my $TM = Turing::Machine->new("UA1.2",6,$pgm,256,' ',3,' 1111101111',256,1000);
      $TM->run();
      check($TM, '9');
   } 
}


{  # ===== Test case UA2 =======================================================
	# Unary Arithmetic 2 : Unary Multiplication
   # For TAPE[>1111X111], EXPECT:HALT[111111111111       >]
	my $pgm =
      "1 ---|11211|1X---|1*---|1A---" .   # Move L to first ' '
		"2 3*2|21---|2X---|2*---|2A---" .   # write '*' here.
		"3 4 1|31312|3X3X2|3*3*2|3A3A2" .   # Move R to first ' '
		"4 ---|415 1|4X5X1|4*---|4A---" .   # Erase '1' L of ' ' 
		"5 ---|51511|5X6X1|5*: 2|5A---" .   # & move to L of X
		"6 ---|617A1|6X---|6*9*2|6A6A1" .   # LOOP: Chg. '1' to 'A'  
		"7 812|71711|7X---|7*7*1|7A---" .   # & write '1' L of '*'
		"8 ---|81812|8X6X1|8*8*2|8A8A2" .   # 
		"9 4 1|91912|9X9X2|9*---|9A912" .   # Chg 'A' to '1',GOTO:4
		": 0 2|:1: 2|:X: 2|:*---|:A: 2" ;	# Erase X and R until ' '
   $pgm =~ s/\|//g;
  
   {  # 4 x 3 = 7
      my $TM = Turing::Machine->new("UA2.1",10,$pgm,256,' ',5,'1111X111',256,1000);
      $TM->run();
      check($TM, '12');
   }
   
   {  # 5 x 4 = 20
      my $TM = Turing::Machine->new("UA2.2",10,$pgm,256,' ',5,'11111X1111',256,1000);
      $TM->run();
      check($TM, '20');
   }
}


### END Turing::UnitTest in UnitTest.pl ########################################

