async def test_shorten(service_client):
    original_url = 'http://github.com/'
    shorten_url = 'http://base.url/kxM4i'

    response = await service_client.post(
        '/api/v1/shorten',
        json={
            'original_url': original_url,
        },
    )

    assert response.status == 200
    assert response.json() == {
        'original_url': original_url,
        'shorten_url': shorten_url,
    }


async def test_shorten_collision(service_client, add_urls, pgsql):
    add_urls('http://existed.url', 'kxM4i')

    original_url = 'http://github.com/'
    shorten_url = 'http://base.url/bGORx'

    response = await service_client.post(
        '/api/v1/shorten',
        json={
            'original_url': original_url,
        },
    )

    assert response.status == 200
    assert response.json() == {
        'original_url': original_url,
        'shorten_url': shorten_url,
    }
