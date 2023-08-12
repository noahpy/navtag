
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

cd(){
    builtin cd $(navtag $mfp -t $@)
}

mv(){
    $(which mv) $(navtag $mfp -t $@)
}

cp(){
    $(which cp) $(navtag $mfp -t $@)
}

mkdir(){
    $(which mkdir) $(navtag $mfp -t $@)
}

mc(){
    results=$(navtag $mfp -t $@)
    $(which mkdir) $results
    builtin cd $results
}


_navtag_dir() {
   local cur
   cur=$(navtag $mfp -t $2)
   _filedir -d
   for i in "${!COMPREPLY[@]}"; do
       original_element="${COMPREPLY[$i]}"
       # Remove element if it is the same as the prefix
       if [ "$original_element" == "$prefix" ]; then
            unset 'COMPREPLY[$i]'
            continue
       fi
       new_element="${original_element/${cur}/${2}}"
       COMPREPLY[$i]="$new_element"
   done

   if [[ $2 != *"/"* ]]; then
       while IFS="" read -r line || [ -n "$line" ]; do
            if [[ $line == $2* ]]; then
               COMPREPLY+=( $line )
            fi
       done < <(navtag $mfp -L)
   fi 
}
complete -F _navtag_dir -o nospace cd mc mkdir

_navtag_filedir() {
   local cur
   cur=$(navtag $mfp -t $2)
   _filedir
   for i in "${!COMPREPLY[@]}"; do
       original_element="${COMPREPLY[$i]}"
       # Remove element if it is the same as the prefix
       if [ "$original_element" == "$prefix" ]; then
            unset 'COMPREPLY[$i]'
            continue
       fi
       new_element="${original_element/${cur}/${2}}"
       COMPREPLY[$i]="$new_element"
   done
   if [[ $2 != *"/"* ]]; then
       while IFS="" read -r line || [ -n "$line" ]; do
           if [[ $line == $2* ]]; then
               COMPREPLY+=( $line )
           fi
       done < <(navtag $mfp -L)
   fi
}
complete -F _navtag_filedir -o nospace mv cp


mfp="/home/noah/projects/navtag/marks.txt"
