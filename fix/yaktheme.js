jQuery(function($){
    Drupal.node_edit_protection = {};
    var click = false; // Allow Submit/Edit button
    var edit = false; // Dirty form flag

    Drupal.behaviors.nodeEditProtection = {
        attach : function(context) {
            // If they leave an input field, assume they changed it.
            $(".page-node-webform :input").each(function() {
                $(this).blur(function() {
                    edit = true;
                });
            });

            // Let all form submit buttons through
            $(".page-node-webform input[type='submit']").each(function() {
                $(this).addClass('node-edit-protection-processed');
                $(this).click(function() {
                    click = true;
                });
            });

            // Catch all links and buttons EXCEPT for "#" links
            $("a, button, input[type='submit']:not(.node-edit-protection-processed)")
                .each(function() {
                    $(this).click(function() {
                        // return when a "#" link is clicked so as to skip the
                        // window.onbeforeunload function
                        if (edit && $(this).attr("href") != "#") {
                            return 0;
                        }
                    });
                });

            // Handle backbutton, exit etc.
            window.onbeforeunload = function() {
                // Add CKEditor support
                if (typeof (CKEDITOR) != 'undefined' && typeof (CKEDITOR.instances) != 'undefined') {
                    for ( var i in CKEDITOR.instances) {
                        if (CKEDITOR.instances[i].checkDirty()) {
                            edit = true;
                            break;
                        }
                    }
                }
                if (edit && !click) {
                    click = false;
                    return (Drupal.t("By leaving this page, you will lose all unsaved work."));
                }
            }
        }
    };

    if(window.screen.width < 768 || window.screen.height < 1024){
        // get current and last page numbers
        var currentPage = $(".webform-progressbar-page.current > .webform-progressbar-page-number").text();
        var lastPage = $(".webform-progressbar-page-number:last").text();
        var currentTitle = $(".webform-progressbar-page.current > .webform-progressbar-page-label").text();

        // Don't replace the progress bar if the current title is set to null
        // (avoiding leftover characters)
        if(currentTitle===""){
            return;
        }

        // Replace progress bar with textual element
        $(".webform-progressbar")
            .replaceWith("<h4>"+ currentTitle + ": " + currentPage + " / " + lastPage + "</h5>");
    }

    if ($('body').hasClass('logged-in')) {
        const parse = $('.user-block-connect a:first').text().match(/(.+) \((.*?)\)$/i);
        const data = {
            username: parse ? parse[2] : '',
            account: {
                href: $('.user-block-connect a:first').attr('href'),
                label: parse ? parse[1] : '',
            },
            template: {
                href: $('.user-block-connect a[href*="template"]').attr('href'),
                label: $('.user-block-connect a[href*="template"]').text(),
            },
            newform: {
                href: $('.user-block-connect a[href*="add/form"]').attr('href'),
                label: $('.user-block-connect a[href*="add/form"]').text(),
            },
            myforms: {
                href: $('.user-block-connect a[href$="forms"]').attr('href'),
                label: $('.user-block-connect a[href$="forms"]').text(),
            },
            logout: {
                href: $('.user-block-connect a[href$="logout"]').attr('href'),
                label: $('.user-block-connect a[href$="logout"]').text(),
            },
            users: {
                href: $('.user-block-connect a[href$="people"]').attr('href'),
                label: $('.user-block-connect a[href$="people"]').text(),
            },
            trads: {
                href: $('.user-block-connect a[href$="translate"]').attr('href'),
                label: $('.user-block-connect a[href$="translate"]').text(),
            },
        };

        $('.user-block-connect')
            .after(`
      <div class="navbar-nav">
        <div class="dropdown">
          <button class="button">
            ${data.newform.label}
            <span class="caret"></span>
          </button>
          <ul class="dropdown-menu">
            <li>
              <a href="${data.newform.href}">${data.newform.label}</a>
            </li>
            <li>
              <a href="${data.template.href}">${data.template.label}</a>
            </li>
          </ul>
        </div>
        <div class="dropdown">
          <button class="dropdown-toggle">
            ${data.username}
            <span class="caret"></span>
          </button>
          <ul class="dropdown-menu">
            <li>
              <a href="${data.account.href}">${data.account.label}</a>
            </li>
            <li>
              <a href="${data.myforms.href}">${data.myforms.label}</a>
            </li>
            ${data.users.label ? `<li><a href="${data.users.href}">${data.users.label}</a></li>` : ''}
            ${data.trads.label ? `<li><a href="${data.trads.href}">${data.trads.label}</a></li>` : ''}
            <li>
              <a href="${data.logout.href}">${data.logout.label}</a>
            </li>
          </ul>
        </div>
      </div>
      `);
    } else {
        const data = {
            signup: {
                href: $('.user-block-connect a[href$="register"]').attr('href'),
                label: $('.user-block-connect a[href$="register"]').text(),
            },
            signin: {
                href: $('.user-block-connect a[href$="user"]').attr('href'),
                label: $('.user-block-connect a[href$="user"]').text(),
            },
        };
        $('.user-block-connect')
            .after(`
      <div class="navbar-nav">
        <a
          class="button"
          href="${data.signup.href}"
        >
          ${data.signup.label}
        </a>
        <a
          class="button btn-info"
          href="${data.signin.href}"
        >
          ${data.signin.label}
        </a>
      </div>`);
    }

    if ($('.page-node-share')) {
        $('#block-system-main input, #block-system-main textarea')
            .attr('onclick', 'this.select()');
    }

    if (window.top.location !== window.self.document.location) {
        $('body').addClass('inframe');
    }

    const query = new URLSearchParams(window.location.search)
    if (query.has('theme')
        && query.get('theme').length < 20
        && /[a-z]+/.test(query.get('theme'))) {
        $('body').addClass(query.get('theme'));
    }

    let cookieNoMoreHelp = false;
    if (document.cookie
        && document.cookie.split('; ').find(row => row.startsWith('nomorehelp='))) {
        cookieNoMoreHelp = document.cookie
            .split('; ')
            .find(row => row.startsWith('nomorehelp='))
            .split('=')[1];
    }

    $('#sidebar-first .block').before(`
    <input
      id="help-toggle"
      ${cookieNoMoreHelp === 'true' ? '' : 'checked="checked"'}
      onchange="document.cookie = 'nomorehelp=' + !this.checked"
      type="checkbox"
    />
    <label for="help-toggle">Afficher/Masquer l’aide</label>`); /* TODO: i18n */

    setInterval(() => { /* TODO make it works with Drupal.behaviors */
        if ($('.page-node-add-form1')
            || $('.page-node-edit.node-type-form1')) {
            if ($('#edit-field-form1-template-und-1')) {
                $('#edit-field-form1-template-und-1').is(':checked')
                    ? $('#edit-field-form1-template-desc').show()
                    : $('#edit-field-form1-template-desc').hide();

                $('#edit-field-form1-template-und-1').on('change', () => {
                    $('#edit-field-form1-template-und-1').is(':checked')
                        ? $('#edit-field-form1-template-desc').show()
                        : $('#edit-field-form1-template-desc').hide();
                });
            }
        }
    }, 1000);
});
