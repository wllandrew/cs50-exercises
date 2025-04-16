select
    title
from
    movies
join
    stars
    on stars.movie_id = movies.id
join
    people
    on stars.person_id = people.id
where
    name = 'Bradley Cooper'
and
    title in
    (
        select
            title
        from
            movies
        join
            stars
            on stars.movie_id = movies.id
        join
            people
            on stars.person_id = people.id
        where
            name = 'Jennifer Lawrence'
    );
