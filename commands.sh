
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

