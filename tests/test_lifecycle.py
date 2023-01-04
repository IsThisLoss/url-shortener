async def test_lifecycle(service_client):
    original_url = 'http://github.com/'

    response = await service_client.post(
        '/api/v1/shorten',
        json={
            'original_url': original_url,
        },
    )

    assert response.status == 200
    shorten_url = response.json()['shorten_url'].split('/')[-1]

    response = await service_client.get(
        f'/u/{shorten_url}',
        allow_redirects=False,
    )

    assert response.status == 301
    assert response.headers['Location'] == original_url
