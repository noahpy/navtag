
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


navtag_sort_args(){
    navtag_pos_args=""
    navtag_opt_args=""
    for arg in "$@"; do
        if [[ $arg == -* ]]; then
            navtag_opt_args+="$arg "
        else
            navtag_pos_args+="$arg "
        fi
    done
}

cd(){
    navtag_sort_args $@
    builtin cd $navtag_opt_args $(navtag $mfp -t $navtag_pos_args)
}

mv(){
    navtag_sort_args $@
    $(which mv) $navtag_opt_args $(navtag $mfp -t $navtag_pos_args)
}

cp(){
    navtag_sort_args $@
    $(which cp) $navtag_opt_args $(navtag $mfp -t $navtag_pos_args)
}

mkdir(){
    navtag_sort_args $@
    $(which mkdir) $navtag_opt_args $(navtag $mfp -t $navtag_pos_args)
}

mc(){
    navtag_sort_args $@
    results=$(navtag $mfp -t $navtag_pos_args)
    $(which mkdir) $results
    builtin cd $results
}

touch(){
    navtag_sort_args $@
    $(which touch) $navtag_opt_args $(navtag $mfp -t $navtag_pos_args)
}


_navtag_label(){
    while IFS="" read -r line || [ -n "$line" ]; do
            if [[ $line == $2* ]]; then
               COMPREPLY+=( $line )
            fi
       done < <(navtag $mfp -L)
}

complete -F _navtag_label -o nospace unmark


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
       # Replace prefix with label
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
       # Replace prefix with label
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
complete -F _navtag_filedir -o nospace mv cp touch


mfp="/home/noah/projects/navtag/marks.txt"
