
function mark(){
    if [ $# -eq 1 ]
    then
        navtag $mfp -a $1 $(pwd)
    fi

    if [ $# -gt 1 ]
    then
        navtag $mfp -a $1 $2
    fi
}

function unmark(){
    navtag $mfp -d $1
}

function marks(){
    navtag $mfp -l
}

function cdx(){
    cd $(navtag $mfp -t $@)
}

function mvx(){
    mv $(navtag $mfp -t $@)
}

function cpx(){
    cp $(navtag $mfp -t $@)
}

function mkx(){
    mkdir $(navtag $mfp -t $@)
}

function mcx(){
    results=$(navtag $mfp -t $@)
    mkdir $results
    cd $results
}



_navtag_dir() {
   local cur
   cur=$(navtag $mfp -t $2)
   _filedir -d
   if [[ $2 != *"/"* ]]; then
       while IFS="" read -r line || [ -n "$line" ]; do
           COMPREPLY+=( "$(echo $line)" )
       done < <(navtag $mfp -L)
   fi
}
complete -F _navtag_dir cdx mcx

_navtag_filedir() {
   local cur
   cur=$(navtag $mfp -t $2)
   _filedir
   if [[ $2 != *"/"* ]]; then
       while IFS="" read -r line || [ -n "$line" ]; do
           COMPREPLY+=( "$(echo $line)" )
       done < <(navtag $mfp -L)
   fi
}
complete -F _navtag_filedir mvx cpx

