#!/usr/bin/wish

package require tclua

#set r [lua {return 1,2,3,4,"allo",nil,5.5,-4.6,true,false,blub}]
#puts "res is $r"
#puts "length is [llength $r]"

#lassign [lua {return 456*93,888}] a b
#puts "$a and $b"

#puts [lua a=123]
#puts [lua bonjour]
#puts [lua return 'allo']
#puts [lua {return 1,true,false}]
#lua {return nil,555}
#lua {return 1,2,4.5}
#lua {return {4,5,6}}
#lua {return function() end}
#lua {n='allo';return n}

package require dohelp
#source dohelp.tcl

#
# config globale pour la documentation
#
set docsDirectory ~/.luacad/docs
set refDocsDirectory /usr/local/share/luacad/docs


#
# Pour lua
#
set wid [lua {return require("editor-csg")}]
puts "tcl got wid $wid"
toplevel .e -use $wid -padx 10 -pady 10 -takefocus 1
wm geometry . 100x50+680+50

#toplevel .e -padx 10 -pady 10 -takefocus 1

.e config -background "#606060"

#### editor status
proc resetEditorStatus {} {
    global editorStatus
    set editorStatus {}
    .status config -bg #c9c9c9
}

proc setEditorStatus {msg} {
    global editorStatus
    set editorStatus $msg
    .status config -bg yellow
}


proc runCheck {} {
    resetEditorStatus
    .t tag remove error 1.0 end
    set code [.t get 1.0 end]
    set res [luacheck $code]
    if { $res=={} } { return 0 }
    # [string "..."]:3: ...
    regexp {\[(.*)\]:([0-9]*): (.*)} $res bidon bidon linenum msg
    .t tag add error $linenum.0 $linenum.end
    .t see $linenum.0
    setEditorStatus $msg
    return 1
}

proc runPreview {} {
    resetEditorStatus
    .t tag remove error 1.0 end
    set code [.t get 1.0 end]
    lua {mode="preview"}
    set res [lua $code]
}

proc runRender {} {
    resetEditorStatus
    .t tag remove error 1.0 end
    set code [.t get 1.0 end]
    lua {mode="render"}
    set res [lua $code]
}

proc runClearCache {} {
    lua {cache.flush()}
}

proc autoincremente {delta} {
        set txt [.t get sel.first sel.last]
        .t replace sel.first sel.last [expr $txt+$delta] sel
        runPreview
}

# empty section = use current word
proc documentation {chapter section {window {}}} {
    global currentHelpDocument
    if { $section=={} } {
        set section [string trim [$window get "insert wordstart" "insert wordend"]]
        set section [string trim [$window get "insert wordstart" "insert wordend"]]
        if { ![string is alnum -strict $section] } {
            puts ">>>$section<<< not usable in documentation"
            set currentHelpDocument {}
            return
        }
    }

    lassign [helpSetDoc $chapter $section] txt tags currentHelpDocument
    puts "help file name is $currentHelpDocument"
    .help.t delete 1.0 end
    .help.t insert 1.0 $txt
    foreach {t from to} $tags { .help.t tag add $t $from $to }
}

proc setupEditeur {} {
    frame .top
    button .top.b0 -text "Check (F4)" -command runCheck
    button .top.b1 -text "Preview (F5)" -command runPreview
    button .top.b2 -text "Render (F6)" -command runRender
    button .top.b3 -text "ClearCache" -command runClearCache
    pack .top.b0 .top.b1 .top.b2 .top.b3 -side left

    label .status -textvariable editorStatus 
    pack .status -expand yes -fill x -padx 10 -pady 5
    resetEditorStatus

    frame .editor
    text .t -yscrollcommand ".scroll set" -setgrid true -width 40 -height 10 -wrap word
    scrollbar .scroll -command ".t yview"
    pack .top
    pack .editor -expand yes -fill both
    pack .scroll -side right -fill y -in .editor
    pack .t -expand yes -fill both -in .editor
    .t config -width 100
    .t config -height 37
    focus .t

    # aide
    frame .help
    text .help.t -yscrollcommand ".help.scroll set" -setgrid true -width 40 -height 10 -wrap word -bg lightyellow
    scrollbar .help.scroll -command ".help.t yview"
    pack .help.scroll -side right -fill y
    pack .help.t -expand yes -fill both

    pack .help -expand yes -fill x
    .help.t tag configure h1 -font "helvetica 18 bold" -relief raised
    .help.t tag configure h2 -font "helvetica 16 bold" -relief raised
    .help.t tag configure a -background yellow
    .help.t tag configure code -font courrier
    .help.t tag configure b -font bold


    # en general, F4=Check, F5=Preview, F6=Render
    bind . <Key-F4> { .top.b0 invoke }
    bind . <Key-F5> { .top.b1 invoke }
    bind . <Key-F6> { .top.b2 invoke }
    ## F8/F9 increment/decremente
    bind .t <F8> { autoincremente 1 }
    bind .t <F9> { autoincremente -1 }
    bind .t <Shift-F8> { autoincremente 0.1 }
    bind .t <Shift-F9> { autoincremente -0.1 }
    # documentation key
    bind .t <F1> { documentation ref {} .t}
    bind .help.t <F1> { documentation ref {} .help.t}


    # Set up the tags 
    .t tag configure error -background yellow
#    .t tag configure bold_italics -font {-family courier -size 12 -weight bold -slant italic}
#    .t tag configure big -font {-family helvetica -size 24 -weight bold}
#    .t tag configure color1 -foreground red
#    .t tag configure sunken -relief sunken -borderwidth 1
#    .t tag bind Ouch <1> {.t insert end "Ouch! "}

    # Now insert text that has the property of the tags
#    .t insert end "Here are a few text styles.\n"
#    .t insert end "1. Bold italic text.\n" bold_italics
#    .t insert end "2. Larger Helvetica  text.\n" big
#    .t insert end "3. Red text.\n" color1
#    .t insert end "4. Sunken text.\n" sunken
#    button .b -text "5. An embedded Hello Button" -command {.t insert end "Hello "}
#    #.t window create end -window .b
#    .t insert end "\n"
#    .t insert end "6. Text with a binding (try clicking me)" Ouch
#    .t insert end "\nAnd all this is editable!"
#
#    scale .scale -orient horizontal -length 50 -from 0 -to 100 -command "puts" 
#    .t insert end "\ntranslate{x="
#    #.t window create end -window .scale
#    .t insert end ",y=10,z=0}{...}"
}


##
##
## Gestion des fichiers
##
##


set fileName {}
wm title . $fileName

# open fname1 , or fname2 if fname1 does not exists
# the used name in the end is always fname1
proc openFileAlternate {fname1 fname2} {
    if { [file exists $fname1] } {
        openFile $fname1
    } else {
        openFile $fname2
        # reset to firstname
        global fileName
        set fileName $fname1
        wm title . $fileName
    }
}

proc openFile {fname} {
    global fileName
    if { $fname == {} } { return }

    if { [catch {set f [open $fname r]}] } {
        tk_messageBox -message "Unable to open file\n$fname" -icon error -type ok -detail "application exit"
        return
    }
    # get file, line by line, and execute
    .t delete 1.0 end
    while {[gets $f line] >= 0} { .t insert end "$line\n" }
    close $f
    .t edit modified false
    set fileName $fname
    wm title . $fileName
}

proc menuOpen {{fname {}}} {
    global fileName
    set mod [.t edit modified]
    if { $mod } {
        set reponse [tk_messageBox -message "Save changes to $fileName ?" -icon question -type yesnocancel -detail "application exit"]
        switch -- $reponse {
            yes menuSave
            no {}
            cancel return
        }
    }
    if { $fname == {} } {
        set types {
              {{lua}       {.lua}      }
              {{All Files}        *             }
              }
        set fname [tk_getOpenFile -filetypes $types]
    }

    openFile $fname
}

proc menuNew {} {
    global fileName
    .t delete 1.0 end
    focus .t
    set fileName {}
    wm title . $fileName
}

proc menuSave {} {
    global fileName
    puts "save! $fileName"
    if { ![.t edit modified] } {
        puts "save : nothing to save"
    } else {
        if { $fileName == {} } {
            menuSaveAs
        } else {
            # create directory, in cas we save to local help files
            file mkdir [file dirname $fileName]
            if { [catch {set f [open $fileName w]}] } {
                tk_messageBox -message "Unable to open file\n$fileName" -icon error -type ok -detail "for writing"
                return
            }
            set dump [.t get 1.0 end]
            puts $dump
            puts $f $dump
            close $f
            .t edit modified false
            puts "save : modified to $fileName"
        }
    }
}

proc menuSaveHelp {} {
    global currentHelpDocument
    puts "Save Help!!! out name is $currentHelpDocument"
    set helpTxt [helpGetDoc .help.t]
    puts "--- help doc $currentHelpDocument ---"
    puts $helpTxt
    puts "----------------------"
    helpSaveDoc $helpTxt $currentHelpDocument
}

proc menuSaveAs {} {
    global fileName
    set types {
      {{lua}       {.lua}      }
      {{All Files}        *             }
      }
    set fname [tk_getSaveFile -filetypes $types -confirmoverwrite true]
    if { $fname == {} } { puts "canceled" ; return }
    if { [catch {set f [open $fname w]}] } {
        tk_messageBox -message "Unable to open file\n$fname" -icon error -type ok -detail "for writing"
        return
    }
    set dump [.t get 1.0 end]
    puts $dump
    puts $f $dump
    close $f
    .t edit modified false
    set fileName $fname
    wm title . $fileName
    puts "saveas : modified to $fileName"
}

proc menuExit {} {
    destroy .
}



proc setupMenu {} {
    menu .m
    . configure -menu .m
    global docsDirectory refDocsDirectory

    # docs/examples/<<chapter>>/<<examples>>
    # convention:  [chapitre]-[num]-[description].lua
    # pas de '-' sauf la.
    set exs1 {}
    set exs2 {}
    catch {set exs1 [glob -tails -directory $docsDirectory/examples *.lua]}
    catch {set exs2 [glob -tails -directory $refDocsDirectory/examples *.lua]}
    set exs [lsort -unique [concat $exs1 $exs2]]


    menu .m.examples -tearoff 0
    foreach i $exs {
        lassign [split $i -] chapter num desc
        lassign [split $desc .] desc ext
        puts "$chapter : $num : $desc . $ext"
        if { ![info exists chaptab($chapter)] } { set chaptab($chapter) {} } 
        lappend chaptab($chapter) [list [string trimleft $num 0] $desc $i]
    }

    set i 0
    foreach {k v} [array get chaptab] {
        puts "adding chapter $k : [lsort -real -index 0 $v]"
        #.m.examples add command -label "$i" -command "menuOpen $i"
        menu .m.examples.e$i -tearoff 0
        foreach ndf [lsort -real -index 0 $v] {
            lassign $ndf num desc fname
            .m.examples.e$i add command -label "$num [string totitle $desc]" -command [list openFileAlternate "$docsDirectory/examples/$fname" "$refDocsDirectory/examples/$fname"]
        }
        .m.examples add cascade -menu .m.examples.e$i -label [string totitle $k]
        incr i
    }

    #foreach i $exs {
    #    .m.examples add command -label "$i" -command "menuOpen $i"
    #}


    menu .m.file -tearoff 0
    .m.file add command -label "New" -command menuNew
    .m.file add command -label "Open" -command menuOpen
    .m.file add command -label "Save" -command menuSave
    .m.file add command -label "SaveAs" -command menuSaveAs
    .m.file add separator
    .m.file add command -label "Exit" -command menuExit

    menu .m.help -tearoff 0
    .m.help add command -label "Intro" -command "documentation guide intro"
    .m.help add command -label "Howto" -command "documentation guide howto"
    .m.help add cascade -menu .m.examples -label "Examples"
    .m.help add separator
    .m.help add command -label "Function sous le curseur (F1)" -command "documentation {} {}"
    .m.help add separator
    .m.help add command -label "Save Help" -command menuSaveHelp


    .m add cascade -menu .m.file -label "File"
    .m add cascade -menu .m.help -label "Help"
}

##### input args
puts "got $argc args"
set preload ""
for {set i 0} {$i<$argc} {incr i} {
    set a [lindex $argv $i]
    if { $a=="-i" && [expr $i+1]<$argc } {
        set preload [lindex $argv [expr $i+1]]
        puts "preload is $preload"
    }
    puts "argument $i is $a"
}

setupEditeur
setupMenu


proc setupOverlay {w} {
    button .e.b1 -text "Bonjour" -command {puts Bonjour}
    button .e.quit -text "Quit!" -command {destroy .}
    #pack .e.b1 -ipadx 15 -pady 5
    pack .e.quit
}


setupOverlay .e

lua {displayProducts=nil}

proc refresh {} {
    # verifier si renderer est nil
    lua {coroutine.resume(renderer,displayProducts);displayProducts=nil}
    after 100 [list after 0 refresh]
}


refresh

## preload?
if { $preload != {} } {
    openFile $preload
}


