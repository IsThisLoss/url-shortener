async def test_unshorten(service_client, add_urls):
    original_url = 'http://github.com/'
    shorten_url = 'shorten'

    add_urls(original_url, shorten_url)

    response = await service_client.get(
        f'/u/{shorten_url}',
        allow_redirects=False,
    )

    assert response.status == 301
    assert response.headers['Location'] == original_url


async def test_unshorten404(service_client, pgsql):
    shorten_url = 'shorten'

    response = await service_client.get(
        f'/u/{shorten_url}',
        allow_redirects=False,
    )

    assert response.status == 404
