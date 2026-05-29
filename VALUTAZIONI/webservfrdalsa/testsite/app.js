document.getElementById('upload-form').addEventListener('submit', async (e) => {
  e.preventDefault();
  const form = e.target;
  const formData = new FormData(form);
  const res = await fetch(form.action, {
    method: 'POST',
    body: formData
  });
  const text = await res.text();
  document.getElementById('upload-result').textContent = res.status + ' ' + res.statusText + '\n' + text;
});

document.getElementById('delete-btn').addEventListener('click', async () => {
  const path = document.getElementById('delete-path').value;
  const res = await fetch(path, { method: 'DELETE' });
  const text = await res.text();
  document.getElementById('delete-result').textContent = res.status + ' ' + res.statusText + '\n' + text;
});

document.getElementById('cgi-btn').addEventListener('click', async () => {
  const res = await fetch('/cgi-bin/test.py');
  const text = await res.text();
  document.getElementById('cgi-result').textContent = res.status + ' ' + res.statusText + '\n' + text;
});

document.getElementById('redirect-btn').addEventListener('click', async () => {
  const res = await fetch('/oldpage');
  const text = await res.text();
  document.getElementById('redirect-result').textContent = res.status + ' ' + res.statusText + '\n' + text;
});

document.getElementById('echo-btn').addEventListener('click', async () => {
  const body = document.getElementById('echo-body').value;
  const res = await fetch('/cgi-bin/echo.py', { method: 'POST', body });
  const text = await res.text();
  document.getElementById('echo-result').textContent = res.status + ' ' + res.statusText + '\n' + text;
});
