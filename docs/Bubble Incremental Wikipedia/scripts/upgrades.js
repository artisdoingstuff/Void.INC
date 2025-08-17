(function () {
    const table = document.getElementById('upgradesTable');
    if (!table) return;

    const tbody = table.querySelector('tbody');
    const rows = Array.from(tbody.querySelectorAll('tr'));

    const search = document.getElementById('upgradeSearch');

    const prevTop = document.getElementById('prevPage');
    const nextTop = document.getElementById('nextPage');
    const infoTop = document.getElementById('pageInfo');

    const prevBottom = document.getElementById('prevPageBottom');
    const nextBottom = document.getElementById('nextPageBottom');
    const infoBottom = document.getElementById('pageInfoBottom');

    const pageSize = 10;
    let filtered = rows.slice();
    let page = 1;

    function sanitizeInput(str) {
        return (str || '').replace(/[<>`"'\/\\]/g, '').trim().toLowerCase();
    }

    function applyFilter() {
        const q = sanitizeInput(search.value);
        if (!q) {
            filtered = rows.slice();
        } else {
            filtered = rows.filter(tr => tr.innerText.toLowerCase().includes(q));
        }
        page = 1;
        render();
    }

    function render() {
        rows.forEach(r => r.style.display = 'none');

        const totalPages = Math.max(1, Math.ceil(filtered.length / pageSize));
        if (page > totalPages) page = totalPages;

        const start = (page - 1) * pageSize;
        const end = start + pageSize;
        filtered.slice(start, end).forEach(r => r.style.display = '');

        prevTop.disabled = page <= 1;
        prevBottom.disabled = page <= 1;
        nextTop.disabled = page >= totalPages;
        nextBottom.disabled = page >= totalPages;

        const label = `Page ${page} of ${totalPages} — ${filtered.length} upgrades`;
        infoTop.textContent = label;
        infoBottom.textContent = label;
    }

    function next() { page++; render(); }
    function prev() { page--; render(); }

    search.addEventListener('input', applyFilter);
    nextTop.addEventListener('click', next);
    nextBottom.addEventListener('click', next);
    prevTop.addEventListener('click', prev);
    prevBottom.addEventListener('click', prev);

    render();
})();
