function show(id)
{
    el = document.getElementById(id);
    if (el.style.display == 'none')
    {
        el.style.display = '';
        //el = document.getElementById('more' + id);
        //el.innerHTML = 'less...';
        window.silk.GM_log('showing');
    } else {
        el.style.display = 'none';
        //el = document.getElementById('more' + id);
        //el.innerHTML = 'more...';
        window.silk.GM_log('hiding');
    }
}

show('sidebar');
show('header');
show('footer');