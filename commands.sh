
function mark(){
    if [ $# -eq 1 ]
    then
        navtag "$mfp" -a "$1" $(pwd)
    fi

    if [ $# -gt 1 ]
    then
        navtag "$mfp" -a "$1" "$2"
    fi
}

function unmark(){
    navtag "$mfp" -d "$1"
}

function marks(){
    navtag "$mfp" -l
}


cd(){
    result=$(navtag "$mfp" -t "$@")
    if [[ -z "$result" ]]; then
       cd ~
    fi
    builtin cd "$( echo "$result" | xargs bash -c 'cd "$0" && pwd')"
}

mv(){
     navtag "$mfp" -t "$@" | xargs mv
}


cp(){
     navtag "$mfp" -t "$@" | xargs cp 
}


mkdir(){
     navtag "$mfp" -t "$@" | xargs mkdir
}

mc(){
    result=$(navtag "$mfp" -t "$@")
    if [[ -z "$result" ]]; then
        return
    fi
    echo "$result" | xargs mkdir
    builtin cd "$(echo "$result" | xargs bash -c 'cd "$0" && pwd')"
}

touch(){
    navtag "$mfp" -t "$@" | xargs touch
}


nvim(){
    $(which nvim) $(navtag "$mfp" -t "$@" | xargs bash -c 'echo "$(pwd)/$0"')
}



# Completion just with shortcut labels
_navtag_label(){
    while IFS="" read -r line || [ -n "$line" ]; do
            if [[ $line == $2* ]]; then
               COMPREPLY+=( $line )
            fi
       done < <(navtag "$mfp" -L)
}

complete -F _navtag_label -o nospace unmark

# Completion with shortcut labels + directories
_navtag_dir() {
   local cur
   cur=$(navtag "$mfp" -t "$2")
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
       done < <(navtag "$mfp" -L)
   fi 
}
complete -F _navtag_dir -o nospace cd mc mkdir

# Completion with shortcut labels + directories + files
_navtag_filedir() {
   local cur
   cur=$(navtag "$mfp" -t "$2")
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
       done < <(navtag "$mfp" -L)
   fi
}
complete -F _navtag_filedir -o nospace mv cp touch nvim nv


