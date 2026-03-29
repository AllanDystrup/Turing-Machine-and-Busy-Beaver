#!/usr/bin/perl -w
# Jan.2008 initial v.0.1 Alpha/Pre-release in MKS Toolkit Perl on Windows XP
# March 2026 ported to Strawberry Perl (64 Bit) v.5.42.0.1 on Windows 10

use strict;
use warnings;



# ==============================================================================
# Fixpoint
# f(x): x -> (('x)('x))
# In LISP: 
# define (f x)
#    let y [be] cons "' cons x nil	[y is ('x) ]
#    [return] cons y cons y nil    	[return (('x)('x))]

# In Perl:
my $f = sub { my $x = $_[0]; return "$x('$x')"; };

print &$f('x'), "\n";
print &$f('&$f'), "\n";
print eval &$f('&$f'), "\n";
print eval eval eval eval eval &$f('&$f'), "\n";
print '&$f(\'&$f\')' eq eval eval eval eval eval &$f('&$f'), "\n";

# Run :
# x('x')
# &$f('&$f')
# &$f('&$f')
# &$f('&$f')
# 1


# ==============================================================================
# Gödel
# g(x): x -> (is-unprovable (value-of (('x)('x))))]
# In LISP:
# define (g x) 
#   let (L x y) cons x cons y nil  [Makes x and y into list]
#   (L is-unprovable 
#   (L value-of (L (L "' x) (L "' x))))

# In Perl:
my $is_unprovable = '$is_unprovable';
my $value_of      = '$value_of';
sub cadr { $_[0] =~ /\(\'(.*)\'\)/ and return $1; }

my $L = sub {  my ($x, $y) = @_; return "$x('$y')"; };
my $g = sub {  my $x = $_[0];
               &$L($is_unprovable, &$L($value_of, "$x('$x')")); };

print '&$g(\'x\')   -> ', &$g('x'), "\n";
print '&$g(\'&$g\') -> ', &$g('&$g'), "\n";
print cadr cadr &$g('&$g'), "\n";
print "\n", eval cadr cadr &$g('&$g'); 
print "\n", &$g('&$g') eq eval cadr cadr &$g('&$g');

# Run :
# &$g('x')   -> $is_unprovable('$value_of('x('x')')')
# &$g('&$g') -> $is_unprovable('$value_of('&$g('&$g')')')
# &$g('&$g')
# $is_unprovable('$value_of('&$g('&$g')')')
# 1

=cut
# ==============================================================================
# Turing
# t(x) -> if (halts? (('x)('x))) [then loop] eval (('x)('x)) [else halt] nil
# In LISP:
# define (turing x) 
#   [Insert supposed halting algorithm here.]
#   let (halts? S-exp) false [<=============]
#
#   [Form ('x)]
#   let y [be] cons "' cons x nil [in]
#      [Form (('x)('x))]
#      let z [be] display cons y cons y nil [in]
#         [If (('x)('x)) has a value, then loop forever, otherwise halt]
#         if (halts? z) [then] eval z [loop forever – contradiction!]
#                       [else] nil [halt]

# In Perl:
# (halts? $S_exp)
my $haltsQ;    # return T|F
sub halts_q {
   my $S_exp = $_[0];
   print "\nEvaluating halts_q(\$S_exp),\n\t", "where \$S_exp: $S_exp \n\t",
     'is known to return: ',$haltsQ ? 'true (=halts)' : 'false (=loops)', "\n";
   return $haltsQ;
}

# fixpoint
my $z = sub {  my $x = $_[0]; return "$x('$x')"; };      # z(x) -> (('x)('x))

# turing
my $t; $t = sub {
   my $x  = $_[0];
   my $xx = &$z($x);
   if (halts_q($xx)) { print  "\nlooping "; eval $xx; }  # loop
   else              { return 'halted'; }                # terminate
};

 
print "\n", &$z('x');
#print "\n", &$z('z');   # looping

$haltsQ = 0;   # <=== 0:false (=looping)
print "\n", &$t('&$t');

$haltsQ = 1;   # <=== 1:true  (=halts)
print "\n", &$t('&$t');

# Run:
# Evaluating halts_q($S_exp),
# 	where $S_exp: &$t('&$t') 
# 	is known to return: false (=loops)
# halted
#
# Evaluating halts_q($S_exp),
# where $S_exp: &$t('&$t') 
#	is known to return: true (=halts)
#
# looping
# looping
# looping
# etc.


# ==============================================================================
# Chaitin
#
# In LISP:
# define expression
#       let (fas n) if = n 1 '(is-elegant x)
#                   if = n 2  nil
#                   if = n 3 '(is-elegant yyy)
#                   if = n 4  . . .
#                   [else]    stop 
#
#       let (loop n)
#           let theorem [be] display (fas n)
#           if = nil theorem [then] (loop + n 1)
#           if = stop theorem [then] fas-has-stopped
#           if = is-elegant car theorem
#              if > display size cadr theorem 
#                   display + 356 size fas
#                 [return] eval cadr theorem
#              [else] (loop + n 1)
#           [else] (loop + n 1)
#
#       (loop 1)

=cut

# sub is_elegant {
#   my ($E, $S, $X);                          # E  Expression
#   $S = eval { $E = $_[0];     length $E; }; # S  Size of E    (= actual form)
#   $X = eval { $E =~ s/\s+/ /; length $E; }; # X* minimal size (= standard form)
#   print "$S-$X\n";
#   return $S == $X ? 1 : 0;   # program-size |X*| = the complexity H(X) of X ?
#}

# ----- formal axiomatic system: A -----
# fas(N)  returns nil if the Nth proof is invalid,
#         returns the theorem that was demonstrated if the Nth proof is valid
my $_fas = '
   my ($z4, $z5, $z6);
   $z4 = "~" x 1063 . "<";
   $z5 = "~" x 1062 . "<";
   $z6 = "~" x 1185 . "<";
   $n == 1 ? "is_elegant(\'x\')"                   :  # |X*|=H(X) of X
   $n == 2 ? "nil"                                 :  # 2.proof invalid
   $n == 3 ? "is_elegant(\'yyy\')"                 :  # |Y*|=H(Y) of Y
   $n == 4 ? "is_elegant(\'substr \'$z4\',0\')"    :  # Run2 T+1=B
   $n == 5 ? "is_elegant(\'substr \'$z5\',0\')"    :  # Run3 T=B
   $n == 6 ? "is_elegant(\'substr \'$z6\',0,-1\')" :  # Run4 T>>B
   "stop";
';
sub fas {
   my $n = $_[0];
   return eval $_fas;
}
# ----- split, a la LISP -----
sub _car  { return eval { $_[0] =~ /(.*)(?=\()/;   $1; }; }    # Head
sub _cadr { return eval { $_[0] =~ /\(\'(.*)\'\)/; $1; }; }    # Tail

# ----- Berry’ paradox expression B -----
# Searches through all possible proofs $n in fas until $n proves an assertion :
#    is-elegant(E), in which E's size is larger than fas's size.
# Then B returns E's value as B's value, which, if it actually happened,
#    would contradict the definition of elegance
my $_B = '
   my ($n, $T);
   print "\n", "-" x 40, "\n";
   $n = $_[0];    print "n=$n:\t";        # input Expression
   $T = fas($n);  print "$T\n";           # return Theorem
   if    ( $T eq "nil" )   { B($n+1); }
   elsif ( $T eq "stop" )  { print "fas-has-stopped"; return; }
   elsif ( _car($T) eq "is_elegant" ) {
      my $t = length(_cadr($T));          print "Theorem:\t$t\n";
      my $b = length($_B)+length($_fas);  print "Berry..:\t$b\n";
      if ($t > $b) { print "Value..:\t", eval _cadr($T); B($n+1); }
      else { B($n+1); }
   }
   else { B($n+1); }
';
   
sub B { eval $_B; }
print "Size B:\t", length($_B)+length($_fas), "\n";
B(1);


