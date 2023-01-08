import pathlib

import pytest

from testsuite.databases.pgsql import discover


pytest_plugins = [
    'pytest_userver.plugins',
    'testsuite.databases.pgsql.pytest_plugin',
]


@pytest.fixture(scope='session')
def root_dir():
    """Path to root directory service."""
    return pathlib.Path(__file__).parent.parent


@pytest.fixture(scope='session')
def initial_data_path(root_dir):
    """Path for find files with data"""
    return [
        root_dir / 'postgresql/data',
    ]


@pytest.fixture(scope='session')
def pgsql_local(root_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'url-shortener',  # service name that goes to the DB connection
        [root_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))


@pytest.fixture
def client_deps(pgsql):
    pass


@pytest.fixture
def add_urls(pgsql):
    def wrapper(original_url, short_url):
        cursor = pgsql['url_shortener'].cursor()
        cursor.execute('''
            INSERT INTO url_shortener.urls(
                original_url,
                short_url
            ) VALUES (%s, %s)
        ''', (original_url, short_url))
    return wrapper
