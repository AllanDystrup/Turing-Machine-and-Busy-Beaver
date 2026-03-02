#!/usr/bin/perl -w

################################################################################
#                       CLASS PACKAGE
################################################################################
# Module Turing/Beaver.pm containing the package = class Turing::Beaver
# Access to object data mediated through object->methods() -- not Exporter()
package Turing::Beaver;
use FindBin qw($Bin);               # Include current script dir in @INC 
use lib "$Bin";			    # "$Bin/..";
use Turing::Machine;                # 
@ISA = ("Machine");                 # Subclass Turing::Machine

use strict;
use warnings;
#use diagnostics;
use Data::Dumper;
use Carp;

use Win32::Console::ANSI qw/ Cls Cursor /;
use POSIX;                                         # round, floor, ceil

select((select(STDOUT), $|=1)[0]);                 # autoflush STDOUT



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
   printf( 'BB ' . $self->{_id} . " DESTROYd %s\n", scalar localtime);
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


   # === 'The Beaver' (the BB)
   $self->{_ones}    =  0;                # 
   $self->{_log}     =  0;                # default: just report result
    
   # === The program (state transition definitions)
   $self->{_program} =  $program;         # 
   $self->{_pStates} =  $pStates;         # Num. program states: 0=HALT, 1-...
   $self->{_trans}   =  0;

	$self->{_TM} = Turing::Machine->New(
                              $id, $pStates, $program,            # Program
                              $tCells, $tSet, $tSymbols, $tape,   # Tape
                              $hCells, $hLoop );                  # History
}


################################################################################
# API :                 CLASS FIELD/PROPERTY ACCESSORS (public)
################################################################################


################################################################################
#                       CLASS METHODS (private)
################################################################################



################################################################################
# API :                 CLASS METHODS (public)
################################################################################


1;
