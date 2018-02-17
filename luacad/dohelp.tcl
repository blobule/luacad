#!/usr/bin/wish

# class: guide ou ref

package provide dohelp 1.0

### Attention, utilise la variable globale docsDirectory

proc helpSetDoc {class name} {
    global docsDirectory
	set fname "$docsDirectory/$class/$name"
	if { [file exists $fname] } {
		set f [open $fname r]
		set txt [read $f]
		close $f
	} else {
		set txt "<h1>$name</h1>\nImpossible de trouver le fichier <code>$fname</code>\n"
	}

    set tags {}
    while { [extractTags txt tags] } { }
    fixTags txt tags

	return [list $txt $tags $fname]
}

proc helpSaveDoc {txt fname} {
    set f [open $fname w]
    if { [catch {open $fname w}] } {
        puts "error opening file $fname for writing"
    } else {
        puts $f $txt
	    close $f
    }
}

#
# on retourne un texte modifie avec une liste de tags {tag from to tag from to ...}
# dans le texte original on remplace les tags par un caractere impossible ẃ
# ajoute aux tags
#
proc extractTags {txtName tagsName} {
    upvar $txtName txt
    upvar $tagsName tags
	set r [regexp -indices -inline -all {<([[:alnum:]]+?)>.*?</\1>} $txt ]
	#puts "RESULT: $r"

    #foreach z $r {
    #    lassign $z from to
    #    puts "$z : >> [string range $txt $from $to] <<"
    #}

	### process the results
    # <aaa>blub</aaa> -> @@@@@blub@@@@@@ 
	### <aaa>....</aaa>  10...30 11..13 ->  remove 30..30-2-3   10..10+1+3, ajuste 10..30 -> 10..30-3-2*3
	foreach {match pat} $r {
		lassign $match mfrom mto
		lassign $pat pfrom pto
		# ajuste pour ce qu'on a enlever jusqu'ici
		set thetag [string range $txt $pfrom $pto]
		# the start <..>
		set from $mfrom
		set to [expr $pto+1]
		set txt [string replace $txt $from $to [string repeat ẃ [expr $to-$from+1]]]
		# end first </...>
		set from [expr $mto-2-($pto-$pfrom+1)]
		set to $mto
		set txt [string replace $txt $from $to [string repeat ẃ [expr $to-$from+1]]]
		# ajuste les tags {tag from to}
        set delta [expr $pto-$pfrom+1] 
		lappend tags $thetag [expr $mfrom+$delta+2] [expr $mto-$delta-3]
	}

    return [llength $r]
}

proc fixTags {txtName tagsName} {
    upvar $txtName txt
    upvar $tagsName tags

    # on calcule les positions, en sautant les @ dans le calcule des line.pos
	set line 1
	set pos 0
	for {set i 0} {$i < [string length $txt]} {incr i} {
		set c [string index $txt $i]
		#puts "$i ($line.$pos) >>$c<<"
		set i2pos($i) $line.$pos
		if { "$c" eq "\n" } { incr line; set pos 0 } else {
            if { "$c" != "ẃ" } { incr pos }
        }
	}
	set i2pos($i) end

    # ajuste et transforme les index en line.pos
	set ftags {}
	foreach {t from to} $tags {
		#puts "yo $t $from $to"
		lappend ftags $t $i2pos($from) $i2pos([expr $to+1])
	}
    set tags $ftags

    # remove ẃ
    set txt [string map {ẃ {}} $txt]
    
}


proc compare {a b} {
	lassign [split $a .] aline apos
	lassign [split $b .] bline bpos
	if { $aline < $bline } { return -1 }
	if { $aline > $bline } { return 1 }
	if { $apos < $bpos } { return -1 }
	if { $apos > $bpos } { return 1 }
	return 0
}


proc helpGetDoc {h} {
	set tn [$h tag names]
	puts "names are $tn"

    set tmp [$h get 1.0 end]
    puts $tmp

	set ts {}
    set tmptag {}
	foreach n $tn {
		set t [$h tag ranges $n]
		puts "tags $n is $t"
		foreach {from to} $t {
			lappend ts [list $n $from]
			lappend ts [list "/$n" $to]
            lappend tmptag $n $from $to
		}
	}
	set ts [lsort -index 1 -decreasing -real -command compare $ts]
	puts $ts

	foreach i $ts {
		lassign $i g pos
		$h insert $pos "<$g>"
	}
    set final [$h get 1.0 end]
    $h delete 1.0 end
    $h insert 1.0 "$tmp"

    foreach {n from to} $tmptag {
        $h tag add $n $from $to
    }

    return $final
}


if { 0 } {

# aide
frame .help
text .help.t -yscrollcommand ".help.scroll set" -setgrid true -width 80 -height 20 -wrap word -bg lightyellow
scrollbar .help.scroll -command ".help.t yview"
pack .help.scroll -side right -fill y
pack .help.t -expand yes -fill both
pack .help -expand yes -fill x

#button .b -text "save" -command saveit
#pack .b


proc documentation {} {
    set w [.help.t get "insert wordstart" "insert wordend"]
    puts "documentation : >>$w<<"
}

bind .help.t <F1> { documentation }

set h .help.t

set fp [open "$docsDirectory/guide/intro" r]
set txt [read $fp]
close $fp

puts $txt

$h tag configure highlightline -background yellow -font "helvetica 14 bold" -relief raised
$h tag configure h1 -font "helvetica 18 bold" -relief raised
$h tag configure h2 -font "helvetica 16 bold" -relief raised
$h tag configure b -font "bold"
$h tag configure a -background yellow

proc check {v} {
    upvar $name x
}

$h insert 1.0 $txt

set tags {}
while { [extractTags txt tags] } { puts "YOYOYO" }

fixTags txt tags

$h insert 1.0 "\n-----\n"
$h insert 1.0 $txt

foreach {t from to} $tags {
	puts " adding tag $t from $from to $to"
	$h tag add $t $from $to
}

#
#foreach {t from to} $tags2 {
#	puts "2 adding tag $t from $from to $to"
#	#$h tag add $t $from $to
#}

}
