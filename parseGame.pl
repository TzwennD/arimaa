#!/usr/bin/perl -w
## call: 'perl parseGame allGamesFile gameNum > outfile' to write game in file
## or 'perl parseGame allGamesFile gameNum > ./arimaa' to let play


use Data::Dumper;
use strict;
use warnings;


my $filename = shift;
my $gamenum = shift;

if($gamenum == 0){die "give a larger number!\n";}

my $count = 0;

open FA, "<$filename" or die "can't open $filename\n";
while(<FA>){
    chomp;
    if($count == $gamenum){
        my @G = split '\t';
        my $termination = $G[22];
        if($termination =~ /[g,m,e,r,p]/)
        {}
        else {print "resign\n"; die "Game terminated unexpectedly! Choose another game!\n";}
        my $realgame = $G[27]; # not sure if this is true!
        $realgame =~ s/\\n/ /g;
        my @F= split ' ', $realgame;
        my $steps=0;
        for(my $i=0;$i< scalar @F;$i++){
            my $current = $F[$i];
            if($current =~ /^\d+[w,s,b,g]$/ )
            {
#                print "-------\n";
                if($steps < 4){
                    print "end\n";
                }
                $steps=0;
                next;
            }
            if($current =~ /x/){next;}
            print $current, "\n";
            $steps++;
        }
        last;
    }
    else{
        $count++;
    }
}

if($count == $gamenum){}
    else {print "game number not available\n";}
