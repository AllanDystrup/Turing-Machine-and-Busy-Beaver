#!/usr/bin/perl -w
# Jan.2008 initial v.0.1 Alpha/Pre-release in MKS Toolkit Perl on Windows XP
# March 2026 ported to Strawberry Perl (64 Bit) v.5.42.0.1 on Windows 10


################################################################################
#                       CLASS PACKAGE
################################################################################
# Module Turing/Machine.pm containing the package = class Turing::Machine
# Access to object data mediated through object->methods() -- not Exporter()
package Turing::Machine;

use feature ':5.10';                            # load all Perl5.10 features
use strict;
use warnings;
#use diagnostics;
use Data::Dumper;
use Carp;

use Win32::Console::ANSI qw/ Cls Cursor /;
use List::Util qw(max min);
use POSIX;                                      # round, floor, ceil

select((select(STDOUT), $|=1)[0]);              # autoflush STDOUT



################################################################################
#                       CLASS LIFECYCLE
################################################################################

# ===== Constructor ============================================================
# Class c'tor allocates, blesses and returns a new object (a blessed datastruct)
# Blessing associates referent with package namespace for method lookup by '->';
# -> invocation passes the invocant (instance ref or class string) as 1.param.
# Ie: Object methods are function calls with runtime determination of namespace.
sub new {
   my ($class, @args) = @_;
    
   # Set datastrture $self as ref to new hash for object data
   my $self = {};

   # Mark (bless) datatype (hash) as object into instance or class package;
   # Invoking $instance->method() or class Turing::Machine->Method() will call
   # method() in this package passing the invocant (ref or string) as 1.param.
   bless ($self, ref($class) || $class);
    
   # Initialize object data (hash)
   $self->_init(@_);
    
   # Return ref to object (data (hash))
   return $self;
}


# ===== Destructor =============================================================
# Class D'tor subroutine is called by the runtime when the object's ref.count
# becomes 0, and the referent is garbage collected; Mandatory name: DESTROY.
sub DESTROY {
   #return;
   my $self = shift;
   printf( 'TM ' . $self->{_id} . " DESTROYd %s\n", scalar localtime);
   print '=' x 80 . "\n";
}


# ===== Initializer ============================================================
# Class initializer, - factored out of class constructor:  $self->_init(@_);
# Define and initialize class fields/properties, ie. hash (key,value) pairs.
sub _init {
   # === Args
   my ($self, $class,
       $id, $pStates, $program,           # Program
       $tCells, $tSet, $tSymbols, $tape,  # Tape
       $hCells, $hLoop)                   # History
   = @_;
    
   # === 'La Machine' (the TM)
   $self->{_id}      =  $id || 0;         # Unique ID for this TM
   $self->{_log}     =  0;                # default: just report result
    
   # === The program (state transition definitions)
   $self->{_pStates} =  $pStates || 0;    # Num. program states: 0=HALT, 1-...
   $self->{_pCells}  =  5;                # Num. byte in one TransDef (const:5)
   $self->{_pISta}   =  substr($program,0,1);
   $self->{_pTrans}  =  $self->{_pISta} . substr($program,1,1);
   for (my $i=0; $i < length $program; $i+=$self->{_pCells} )  {
      my $in  = substr($program,  $i,  2);
      my $out = substr($program, $i+2, 3);
      $self->{_program}->{$in} = [$out, 0];
   }   
    
   # === The tape (input/output)
   $self->{_tSymbols}=  $tSymbols || 0;   # Num. tape symbols
   $self->{_tape}    =  $tSet x $tCells;  # TM-tape, reset to all $tSet chars
   $self->{_tCells}  =  $tCells;          # Num. bytes on tape 
   $self->{_tIndex}  =  floor($tCells/2); # Current pos: midde of _tape
   substr($self->{_tape}, $self->{_tIndex}, length $tape, $tape);

   # === The history (transition log)
   $self->{_history} = '@' x $hCells;     # Trace of stateTrans (cyclic buf)
   $self->{_hCells}  =  $hCells;          # Num. bytes on history tape
   $self->{_hIndex}  =  0;                # Idx to current pos in _history log
   $self->{_hTrans}  =  0;                # Current state trans number
   $self->{_hLoop}   =  $hLoop;           # Max state trans number (=Loop omega)
   $self->{_hDelta}  =  0;                # Running time for TM on _tape
  
   # === Postconditions
   _check ($self->{_pStates} * $self->{_tSymbols} * $self->{_pCells} > 0,
         "Error:ProgramSize<=0");
   _check ($self->{_tIndex} <= length $self->{_tape},
         "Error: Input>Tape");
}


################################################################################
# API :                 CLASS FIELD/PROPERTY ACCESSORS (public)
################################################################################
sub id {
   my $self = shift;
   if (@_) { $self->{_id} = shift; }
   return $self->{_id};
}
sub log {
   my $self = shift;
   if (@_) { $self->{_log} = shift; }
   return $self->{_log};
}
sub program {
   my $self = shift;
   if (@_) { $self->{_program} = shift; }
   return $self->{_program};
}
sub tape {
   my $self = shift;
   if (@_) { $self->{_tape} = shift; }
   return $self->{_tape};
}
sub history {
   my $self = shift;
   if (@_) { $self->{_history} = shift; }
   return $self->{_history};
}

sub get_hTrans {
   my $self = shift;
   if (@_) { croak "hTrans RO property"; }
   return $self->{_hTrans};
}
sub get_hDelta{
    my $self = shift;
   if (@_) { croak "elapsed RO property"; }
   return $self->{_hDelta};
}


################################################################################
#                       CLASS METHODS (private)
################################################################################
sub _getPsym {
   my ($self, $key, $pos) = @_;
   return substr($self->{_program}->{$key}[0], $pos, 1);
}
sub _getTsym {
   my ($self, $pos) = @_;
   #_check($self->{_tIndex} >= 0, "Error:tape underflow");
   #_check($self->{_tIndex} < length $self->{_tape}, "Error:tape overflow");
   return substr($self->{_tape}, $self->{_tIndex}+$pos, 1);
}
sub _setTsym {
   my ($self, $pos, $tSym) = @_;
   substr($self->{_tape}, $self->{_tIndex}+$pos, 1, $tSym);
}
sub _setHsym {
   my ($self, $pos,, $hSym) = @_;
   substr($self->{_history}, $self->{_hIndex}+$pos, 1, $hSym);
}

sub _check {
   my ($condition, $errmsg) = @_;
   if ( ! $condition) { croak $errmsg; }
}


################################################################################
# API :                 CLASS METHODS (public)
################################################################################

# ===== Transition (move) ======================================================
sub trans {
   my $self = shift;
   my ($pISta, $tISym, $pOSta, $pOSym, $pOMov, $trans);
   use constant {
      ISTA  => 0,  # input  state
      ISYM  => 1,  #        sym
      OSTA  => 0,  # output state
      OSYM  => 1,  #        symbol
      OMOV  => 2,  #        move
    };

   # === Find trans in program
   $pISta = $self->{_pISta};                          # INPUT program state
   $tISym = $self->_getTsym(0);                       #        tape symbol
   $self->{_pTrans} = $pISta.$tISym;                  #        trans.def.
   $self->{_program}->{$self->{_pTrans}}[1]++;
   
   $pOSta = $self->_getPsym($self->{_pTrans}, OSTA);  # OUTPUT propram state
   $pOSym = $self->_getPsym($self->{_pTrans}, OSYM);  #        tape symbol
   $pOMov = $self->_getPsym($self->{_pTrans}, OMOV);  #        tape move


   # === Record trans in history
   _check( $self->{_hTrans}++ <= $self->{_hLoop},"Error: Loop Omega".$self->{_hTrans} );
   $self->_setHsym(0, $pISta); $self->{_hIndex}++;
   $self->_setHsym(0, $tISym); $self->{_hIndex}++;
   $self->_setHsym(0, '-');    $self->{_hIndex}++;
   if ( $self->{_hIndex}+3 > $self->{_hCells} ) { $self->{_hIndex} = 0; }
   $self->_setHsym(0, '>');


   # === Write output and move tape
   $self->_setTsym(0, $pOSym);                  # Write pgm output to tape
 
   given ( $pOMov ) {                           # move according to $pMov :
      when  (/[0H]/) { ; }                      #  0: (no move)
      when  (/[1L]/) { $self->{_tIndex}--;      #  1: move left
            _check($self->{_tIndex} >= 0, "Error:tape underflow"); }
      when  (/[2R]/) { $self->{_tIndex}++;      #  2: move right
            _check($self->{_tIndex}<$self->{_tCells}, "Error:tape overflow"); }
      default {
            _check (0, "Error: Move must be 0|H:Halt, 1L:Left, 2|R:Right"); }
   }
   
   # === Make program trans (or HALT)
   if ($pOSta =~ /[0H]/) { return 0; }          # State 0 = HALT
   $self->{_pISta} = $pOSta;                    # or continue with new state
   return 1;
}


# ===== Run machine to HALT ====================================================
sub run {
   my ($self, $trace) = @_;
   if (defined $trace) { $self->log($trace); }     # override default 0
   if ($self->log() < 0) { return; }               # don't run!
   
   my $t = time();
   my $l = $self->log() || 0;
   Cls() if $l > 1;      
   do {                                            #  0 (0b----) no trace
      $self->trace($l) if $l > 0;                  #  1 (0b---1) running
      sleep 1              if ($l & 0x2);          #  2 (0b--1-) slow
      my $i=<STDIN>        if ($l & 0x4);          #  4 (0b-1--) step on <return>
      Cursor(1,1)          if ($l & 0x8);          #  8 (0b1---) cursor reset)
   } while ( $self->trans() );
   $self->trace($l) if $self->log();
   
   my ($h,$m,$s) = (gmtime(time()-$t))[2,1,0];
   $self->{_hDelta} = "$h:$m:$s";
}


# ===== Trace state of machine to STDOUT =======================================
sub trace {
   my ($self, $trace) = @_;
   
   # ===== 'La Machine'
   if ($trace & 0x10) {
      print "Machine:\t" . $self->id() . "\n";
      #print Data::Dumper::Dumper($self);
      #foreach my $k (sort keys %{$self}) { print "$k -> $self->{$k} \n"; }
   }

   # ===== Program
   if ($trace & 0x20) {
      print "\nProgram:\n";
      #print print Data::Dumper::Dumper($r_pgm);
      my $res;
      my $r_pgm = $self->{_program};
      foreach my $k (sort keys %{$r_pgm}) {
         $res .= ($k eq $self->{_pTrans} ? "\t>" : "\t ");
         $res .= "$k->$r_pgm->{$k}[0] $r_pgm->{$k}[1]\n";
      }
      print $res;
   }
   
   # ===== History
   if ($trace & 0x40) {
      print "\nTransition:\t" . $self->{_hTrans} . "\n";
      print "History:\t", $self->{_history}, "\n\n";
   }
   
   # ===== Tape
   if ($trace & 0x80) {    # std TM
      print "Tape:\t\t[",
        substr($self->{_tape}, 0, $self->{_tIndex}), "[$self->{_tIndex}]>",
        substr($self->{_tape}, $self->{_tIndex}), "]\n\n";
   }
   if ($trace & 0x100) {  # beaver TM
      my $i = $self->{_tIndex};                       # abs tape index (cf. 0)
      my $j =  $i - floor(length($self->{_tape}))/2;  # rel tape index (cf middle)
      
      my $l = min( max( index($self->{_tape}, '1'), 0), $i);   # size: left tape
      my $r = max( max( rindex($self->{_tape}, '1'), 0), $i);  # size: right tape
      return if ($l==0 and $r==$i);                            # return if all 0's
 
      my $t = substr($self->{_tape}, $l, $r-$l),;                 # tape with 1's 
      my $u = substr($t, 0, $i-$l) . " [$j]>" . substr($t,$i-$l); # left [cur]>right
      print 'Tape(' . $self->{_hTrans} . ' ' .$self->{_pTrans} . "):\t\t",
         "[$l]", $u, "[", length($self->{_tape})-$r, "]\n";
   }

}

1;